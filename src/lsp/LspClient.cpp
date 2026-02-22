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
#include <sstream>

namespace lsp
{

CLspClient::~CLspClient() { Stop(); }

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

bool CLspClient::Start(const std::wstring &serverPath, const std::string &workspaceRoot,
                       const std::vector<std::string> &libraryPaths, const nlohmann::json &settingsJson)
{
    if (!m_process.Start(serverPath, L"--stdio", L""))
    {
        return false;
    }

    // Build initialize params
    nlohmann::json capabilities = {
        {"textDocument",
         {{"synchronization", {{"didSave", true}, {"dynamicRegistration", false}}},
          {"completion",
           {{"completionItem", {{"snippetSupport", false}, {"documentationFormat", {"plaintext"}}}},
            {"dynamicRegistration", false}}},
          {"signatureHelp", {{"dynamicRegistration", false}}},
          {"publishDiagnostics", {{"relatedInformation", false}}}}}};

    // LuaLS-specific settings
    nlohmann::json settings = settingsJson;
    if (!libraryPaths.empty() && !settings.contains("Lua"))
    {
        settings["Lua"]["workspace"]["library"] = libraryPaths;
    }

    nlohmann::json initParams = {
        {"processId", static_cast<int>(GetCurrentProcessId())},
        {"capabilities", capabilities},
        {"rootUri", workspaceRoot.empty() ? nlohmann::json(nullptr) : nlohmann::json("file:///" + workspaceRoot)},
        {"initializationOptions", settings}};

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
        m_process.Kill();
        return false;
    }

    // Send initialized notification
    SendNotification("initialized", nlohmann::json::object());
    m_initialized = true;

    return true;
}

void CLspClient::Stop()
{
    if (!m_initialized)
    {
        return;
    }

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
    m_documentVersions.clear();
}

bool CLspClient::IsRunning() const { return m_initialized && m_process.IsRunning(); }

// ---------------------------------------------------------------------------
// Document operations
// ---------------------------------------------------------------------------

void CLspClient::OpenDocument(const std::string &uri, const std::string &languageId, const std::string &text)
{
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

// ---------------------------------------------------------------------------
// Polling
// ---------------------------------------------------------------------------

void CLspClient::SetDiagnosticsCallback(DiagnosticsCallback callback) { m_diagnosticsCallback = std::move(callback); }

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
        // Error response — discard pending callback
        int id = message["id"].get<int>();
        m_completionCallbacks.erase(id);
        m_signatureHelpCallbacks.erase(id);
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
}

void CLspClient::HandleNotification(const std::string &method, const nlohmann::json &params)
{
    if (method == "textDocument/publishDiagnostics" && m_diagnosticsCallback)
    {
        auto diag = params.get<PublishDiagnosticsParams>();
        m_diagnosticsCallback(diag.uri, std::move(diag.diagnostics));
    }
    // Other notifications (window/logMessage, etc.) are silently ignored
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
    nlohmann::json notification = {{"jsonrpc", "2.0"}, {"method", method}, {"params", params}};
    Send(notification);
}

void CLspClient::Send(const nlohmann::json &message)
{
    std::string encoded = JsonRpc::Encode(message);
    m_process.Write(encoded);
}

} // namespace lsp
