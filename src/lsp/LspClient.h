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

#pragma once

#include "lsp/Api.h"
#include "lsp/JsonRpc.h"
#include "lsp/Protocol.h"
#include "lsp/Transport.h"
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace lsp
{

/// Callback types for asynchronous LSP responses.
using DiagnosticsCallback = std::function<void(const std::string &uri, std::vector<Diagnostic> diagnostics)>;
using CompletionCallback = std::function<void(CompletionList completions)>;
using SignatureHelpCallback = std::function<void(std::optional<SignatureHelp> help)>;
using HoverCallback = std::function<void(std::optional<HoverResult> hover)>;

/// High-level LSP client that manages the language server lifecycle
/// and provides a simple API for document operations.
///
/// All communication is single-threaded: the GUI calls Poll() on a timer
/// to process incoming messages and dispatch callbacks.
class LSP_API CLspClient
{
  public:
    CLspClient() = default;
    ~CLspClient();

    CLspClient(const CLspClient &) = delete;
    CLspClient &operator=(const CLspClient &) = delete;

    /// Start the language server process and perform the LSP handshake.
    /// @param serverPath  Path to the LuaLS executable.
    /// @param workspaceRoot  Root directory of the workspace (mod folder).
    /// @param configPath  Path to a LuaLS config file (passed via --configpath).
    /// @returns true if the server started and initialized successfully.
    bool Start(const std::wstring &serverPath, const std::string &workspaceRoot, const std::wstring &configPath = L"");

    /// Shut down the language server gracefully.
    void Stop();

    /// Check if the language server is running.
    [[nodiscard]] bool IsRunning() const;

    // ----- Document operations -----

    /// Notify the server that a document was opened.
    void OpenDocument(const std::string &uri, const std::string &languageId, const std::string &text);

    /// Notify the server that a document's content changed (full document sync).
    void ChangeDocument(const std::string &uri, const std::string &text);

    /// Notify the server that a document was closed.
    void CloseDocument(const std::string &uri);

    // ----- Request operations (async, results delivered via callbacks) -----

    /// Request completions at the given position.
    void RequestCompletion(const std::string &uri, int line, int character, CompletionCallback callback);

    /// Request signature help at the given position.
    void RequestSignatureHelp(const std::string &uri, int line, int character, SignatureHelpCallback callback);

    /// Request hover information at the given position.
    void RequestHover(const std::string &uri, int line, int character, HoverCallback callback);

    // ----- Polling and callbacks -----

    /// Register a per-document diagnostics callback.
    /// Each editor registers its own callback keyed by document URI.
    void RegisterDiagnosticsCallback(const std::string &uri, DiagnosticsCallback callback);

    /// Unregister a previously registered diagnostics callback.
    void UnregisterDiagnosticsCallback(const std::string &uri);

    /// Read and process any pending messages from the language server.
    /// Must be called periodically (e.g., on a wxTimer at ~100ms interval).
    void Poll();

  private:
    void SendRequest(const std::string &method, const nlohmann::json &params);
    void SendNotification(const std::string &method, const nlohmann::json &params);
    void HandleMessage(const nlohmann::json &message);
    void HandleResponse(int id, const nlohmann::json &result);
    void HandleNotification(const std::string &method, const nlohmann::json &params);

    /// Send a raw JSON-RPC message (adds Content-Length framing).
    void Send(const nlohmann::json &message);

    CProcess m_process;
    JsonRpc m_framing;

    int m_nextRequestId = 1;
    bool m_initialized = false;

    /// Pending request callbacks, keyed by request ID.
    std::map<int, CompletionCallback> m_completionCallbacks;
    std::map<int, SignatureHelpCallback> m_signatureHelpCallbacks;
    std::map<int, HoverCallback> m_hoverCallbacks;

    /// Per-URI diagnostics callbacks (server-initiated notifications).
    std::map<std::string, DiagnosticsCallback> m_diagnosticsCallbacks;

    /// Document version tracking for didChange.
    std::map<std::string, int> m_documentVersions;
};

} // namespace lsp
