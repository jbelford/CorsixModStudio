/*
LSP Client Library
Copyright (C) 2026 CorsixModStudio Contributors

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "lsp/LspClient.h"
#include <rainman/core/RainmanLog.h>
#include <sstream>

namespace lsp
{

CLspClient::~CLspClient() { Stop(); }

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

bool CLspClient::Start(const std::wstring &serverPath, const std::string &workspaceRoot, const std::wstring &configPath)
{
    CDMS_LOG_INFO("LSP: Starting language server...");

    // Build command-line args: always use --stdio, optionally add --configpath
    std::wstring args = L"--stdio";
    if (!configPath.empty())
    {
        args += L" --configpath=\"" + configPath + L"\"";
        // Convert wstring for logging
        std::string configPathUtf8(configPath.begin(), configPath.end());
        CDMS_LOG_INFO("LSP: Using config file: {}", configPathUtf8);
    }

    if (!m_process.Start(serverPath, args, L""))
    {
        CDMS_LOG_ERROR("LSP: Failed to spawn language server process");
        return false;
    }
    CDMS_LOG_INFO("LSP: Process spawned, sending initialize request");

    // Build initialize params
    nlohmann::json capabilities = {
        {"textDocument",
         {{"synchronization", {{"didSave", true}, {"dynamicRegistration", false}}},
          {"completion",
           {{"completionItem", {{"snippetSupport", false}, {"documentationFormat", {"plaintext"}}}},
            {"dynamicRegistration", false}}},
          {"signatureHelp", {{"dynamicRegistration", false}}},
          {"hover", {{"dynamicRegistration", false}, {"contentFormat", {"markdown"}}}},
          {"publishDiagnostics", {{"relatedInformation", false}}}}}};

    nlohmann::json initParams = {
        {"processId", static_cast<int>(GetCurrentProcessId())},
        {"capabilities", capabilities},
        {"rootUri", workspaceRoot.empty() ? nlohmann::json(nullptr) : nlohmann::json("file:///" + workspaceRoot)}};

    // Send initialize request (id=0, special)
    nlohmann::json initRequest = {{"jsonrpc", "2.0"}, {"id", 0}, {"method", "initialize"}, {"params", initParams}};
    Send(initRequest);

    // Wait for initialize response (blocking, with timeout)
    constexpr int kMaxWaitMs = 10000;
    constexpr int kPollIntervalMs = 50;
    int waited = 0;
    bool gotResponse = false;

    while (waited < kMaxWaitMs)
    {
        std::string data = m_process.Read();
        if (!data.empty())
        {
            m_framing.Feed(data);
        }

        while (auto msg = m_framing.TryParse())
        {
            if (msg->contains("id") && (*msg)["id"].get<int>() == 0)
            {
                gotResponse = true;
                break;
            }
        }

        if (gotResponse)
        {
            break;
        }

        Sleep(kPollIntervalMs);
        waited += kPollIntervalMs;
    }

    if (!gotResponse)
    {
        CDMS_LOG_ERROR("LSP: Initialize handshake timed out after {}ms", kMaxWaitMs);
        m_process.Kill();
        return false;
    }

    // Send initialized notification
    SendNotification("initialized", nlohmann::json::object());

    m_initialized = true;

    CDMS_LOG_INFO("LSP: Language server initialized successfully");
    return true;
}

void CLspClient::Stop()
{
    if (!m_initialized)
    {
        return;
    }

    CDMS_LOG_INFO("LSP: Shutting down language server");
    m_initialized = false;

    // Send shutdown request
    nlohmann::json shutdownRequest = {{"jsonrpc", "2.0"}, {"id", m_nextRequestId++}, {"method", "shutdown"}};
    Send(shutdownRequest);

    // Brief wait for response
    Sleep(200);

    // Send exit notification
    SendNotification("exit", nlohmann::json::object());

    // Give the server a moment to exit gracefully
    Sleep(200);

    m_process.Kill();
    m_framing.Reset();
    m_completionCallbacks.clear();
    m_signatureHelpCallbacks.clear();
    m_hoverCallbacks.clear();
    m_diagnosticsCallbacks.clear();
    m_documentVersions.clear();
}

bool CLspClient::IsRunning() const { return m_initialized && m_process.IsRunning(); }

// ---------------------------------------------------------------------------
// Document operations
// ---------------------------------------------------------------------------

void CLspClient::OpenDocument(const std::string &uri, const std::string &languageId, const std::string &text)
{
    CDMS_LOG_INFO("LSP: Opening document: {} (lang={}, {} bytes)", uri, languageId, text.size());
    m_documentVersions[uri] = 1;

    DidOpenTextDocumentParams params;
    params.textDocument.uri = uri;
    params.textDocument.languageId = languageId;
    params.textDocument.version = 1;
    params.textDocument.text = text;

    SendNotification("textDocument/didOpen", params);
}

void CLspClient::ChangeDocument(const std::string &uri, const std::string &text)
{
    int &version = m_documentVersions[uri];
    ++version;

    DidChangeTextDocumentParams params;
    params.textDocument.uri = uri;
    params.textDocument.version = version;
    params.contentChanges.push_back({text});

    SendNotification("textDocument/didChange", params);
}

void CLspClient::CloseDocument(const std::string &uri)
{
    CDMS_LOG_DEBUG("LSP: Closing document: {}", uri);
    DidCloseTextDocumentParams params;
    params.textDocument.uri = uri;

    SendNotification("textDocument/didClose", params);
    m_documentVersions.erase(uri);
}

// ---------------------------------------------------------------------------
// Requests
// ---------------------------------------------------------------------------

void CLspClient::RequestCompletion(const std::string &uri, int line, int character, CompletionCallback callback)
{
    int id = m_nextRequestId++;
    m_completionCallbacks[id] = std::move(callback);

    CompletionParams params;
    params.textDocument.uri = uri;
    params.position.line = line;
    params.position.character = character;

    nlohmann::json request = {
        {"jsonrpc", "2.0"}, {"id", id}, {"method", "textDocument/completion"}, {"params", params}};
    Send(request);
}

void CLspClient::RequestSignatureHelp(const std::string &uri, int line, int character, SignatureHelpCallback callback)
{
    int id = m_nextRequestId++;
    m_signatureHelpCallbacks[id] = std::move(callback);

    SignatureHelpParams params;
    params.textDocument.uri = uri;
    params.position.line = line;
    params.position.character = character;

    nlohmann::json request = {
        {"jsonrpc", "2.0"}, {"id", id}, {"method", "textDocument/signatureHelp"}, {"params", params}};
    Send(request);
}

void CLspClient::RequestHover(const std::string &uri, int line, int character, HoverCallback callback)
{
    int id = m_nextRequestId++;
    m_hoverCallbacks[id] = std::move(callback);

    HoverParams params;
    params.textDocument.uri = uri;
    params.position.line = line;
    params.position.character = character;

    nlohmann::json request = {{"jsonrpc", "2.0"}, {"id", id}, {"method", "textDocument/hover"}, {"params", params}};
    Send(request);
}

// ---------------------------------------------------------------------------
// Polling
// ---------------------------------------------------------------------------

void CLspClient::RegisterDiagnosticsCallback(const std::string &uri, DiagnosticsCallback callback)
{
    m_diagnosticsCallbacks[uri] = std::move(callback);
}

void CLspClient::UnregisterDiagnosticsCallback(const std::string &uri) { m_diagnosticsCallbacks.erase(uri); }

void CLspClient::Poll()
{
    if (!m_initialized)
    {
        return;
    }

    std::string data = m_process.Read();
    if (!data.empty())
    {
        m_framing.Feed(data);
    }

    while (auto msg = m_framing.TryParse())
    {
        HandleMessage(*msg);
    }
}

// ---------------------------------------------------------------------------
// Message handling
// ---------------------------------------------------------------------------

void CLspClient::HandleMessage(const nlohmann::json &message)
{
    if (message.contains("id") && message.contains("result"))
    {
        HandleResponse(message["id"].get<int>(), message["result"]);
    }
    else if (message.contains("id") && message.contains("error"))
    {
        // Error response — log and discard pending callback
        int id = message["id"].get<int>();
        auto errObj = message["error"];
        CDMS_LOG_WARN("LSP: Error response id={}: {} (code={})", id, errObj.value("message", "unknown"),
                      errObj.value("code", -1));
        m_completionCallbacks.erase(id);
        m_signatureHelpCallbacks.erase(id);
        m_hoverCallbacks.erase(id);
    }
    else if (message.contains("method"))
    {
        std::string method = message["method"].get<std::string>();
        nlohmann::json params = message.value("params", nlohmann::json::object());
        HandleNotification(method, params);
    }
}

void CLspClient::HandleResponse(int id, const nlohmann::json &result)
{
    // Check completion callbacks
    auto compIt = m_completionCallbacks.find(id);
    if (compIt != m_completionCallbacks.end())
    {
        CompletionList list = result.get<CompletionList>();
        auto cb = std::move(compIt->second);
        m_completionCallbacks.erase(compIt);
        cb(std::move(list));
        return;
    }

    // Check signature help callbacks
    auto sigIt = m_signatureHelpCallbacks.find(id);
    if (sigIt != m_signatureHelpCallbacks.end())
    {
        auto cb = std::move(sigIt->second);
        m_signatureHelpCallbacks.erase(sigIt);
        if (result.is_null())
        {
            cb(std::nullopt);
        }
        else
        {
            cb(result.get<SignatureHelp>());
        }
        return;
    }

    // Check hover callbacks
    auto hoverIt = m_hoverCallbacks.find(id);
    if (hoverIt != m_hoverCallbacks.end())
    {
        auto cb = std::move(hoverIt->second);
        m_hoverCallbacks.erase(hoverIt);
        if (result.is_null())
        {
            cb(std::nullopt);
        }
        else
        {
            cb(result.get<HoverResult>());
        }
        return;
    }
}

void CLspClient::HandleNotification(const std::string &method, const nlohmann::json &params)
{
    if (method == "textDocument/publishDiagnostics")
    {
        auto diag = params.get<PublishDiagnosticsParams>();
        CDMS_LOG_DEBUG("LSP: Received {} diagnostics for {}", diag.diagnostics.size(), diag.uri);
        auto it = m_diagnosticsCallbacks.find(diag.uri);
        if (it != m_diagnosticsCallbacks.end())
        {
            it->second(diag.uri, std::move(diag.diagnostics));
        }
    }
    else if (method == "window/logMessage")
    {
        // Forward LuaLS log messages to our log
        int type = params.value("type", 4);
        std::string msg = params.value("message", "");
        if (type <= 1)
        {
            CDMS_LOG_ERROR("LuaLS: {}", msg);
        }
        else if (type == 2)
        {
            CDMS_LOG_WARN("LuaLS: {}", msg);
        }
        else if (type == 3)
        {
            CDMS_LOG_INFO("LuaLS: {}", msg);
        }
        else
        {
            CDMS_LOG_DEBUG("LuaLS: {}", msg);
        }
    }
    else
    {
        CDMS_LOG_TRACE("LSP: Ignoring notification: {}", method);
    }
}

// ---------------------------------------------------------------------------
// Wire-level helpers
// ---------------------------------------------------------------------------

void CLspClient::SendRequest(const std::string &method, const nlohmann::json &params)
{
    nlohmann::json request = {{"jsonrpc", "2.0"}, {"id", m_nextRequestId++}, {"method", method}, {"params", params}};
    Send(request);
}

void CLspClient::SendNotification(const std::string &method, const nlohmann::json &params)
{
    CDMS_LOG_TRACE("LSP: Sending notification: {}", method);
    nlohmann::json notification = {{"jsonrpc", "2.0"}, {"method", method}, {"params", params}};
    Send(notification);
}

void CLspClient::Send(const nlohmann::json &message)
{
    std::string encoded = JsonRpc::Encode(message);
    m_process.Write(encoded);
}

} // namespace lsp
