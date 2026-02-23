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
#include <atomic>
#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <vector>

namespace lsp
{

/// Callback types for asynchronous LSP responses.
using DiagnosticsCallback = std::function<void(const std::string &uri, std::vector<Diagnostic> diagnostics)>;
using CompletionCallback = std::function<void(CompletionList completions)>;
using SignatureHelpCallback = std::function<void(std::optional<SignatureHelp> help)>;
using HoverCallback = std::function<void(std::optional<HoverResult> hover)>;
using DefinitionCallback = std::function<void(std::optional<Location> location)>;
using ReadyCallback = std::function<void(bool success)>;
using WorkspaceLoadedCallback = std::function<void()>;

/// High-level LSP client that manages the language server lifecycle
/// and provides a simple API for document operations.
///
/// Communication uses a dedicated I/O background thread for pipe reads/writes
/// and JSON-RPC processing. The public API is called from the UI thread.
/// Poll() must be called periodically on the UI thread to dispatch callbacks.
class LSP_API CLspClient
{
  public:
    CLspClient() = default;
    ~CLspClient();

    CLspClient(const CLspClient &) = delete;
    CLspClient &operator=(const CLspClient &) = delete;

    /// Start the language server asynchronously.
    /// Spawns the process and I/O thread, returns immediately.
    /// When initialization completes (or fails), @p onReady is delivered via Poll().
    void StartAsync(const std::wstring &serverPath, const std::string &workspaceRoot,
                    const std::wstring &configPath = L"", ReadyCallback onReady = nullptr);

    /// Blocking start — calls StartAsync() and waits until ready.
    /// Provided for backward compatibility and tests.
    /// @returns true if the server initialized successfully.
    bool Start(const std::wstring &serverPath, const std::string &workspaceRoot, const std::wstring &configPath = L"");

    /// Shut down the language server gracefully.
    /// Signals the I/O thread to stop, joins it, then cleans up. Non-sleeping.
    void Stop();

    /// Check if the server has completed initialization and is ready for requests.
    [[nodiscard]] bool IsReady() const;

    /// Check if the language server process is alive (may still be initializing).
    [[nodiscard]] bool IsRunning() const;

    /// Check if the server has finished loading workspace libraries.
    /// Returns true after all $/progress tokens have completed, or immediately
    /// if the server never sent any progress tokens (fast workspace load).
    [[nodiscard]] bool IsWorkspaceLoaded() const;

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

    /// Request go-to-definition at the given position.
    void RequestDefinition(const std::string &uri, int line, int character, DefinitionCallback callback);

    // ----- Polling and callbacks -----

    /// Register a per-document diagnostics callback.
    /// Each editor registers its own callback keyed by document URI.
    void RegisterDiagnosticsCallback(const std::string &uri, DiagnosticsCallback callback);

    /// Unregister a previously registered diagnostics callback.
    void UnregisterDiagnosticsCallback(const std::string &uri);

    /// Register a callback to be invoked when workspace loading completes.
    /// Keyed by a caller-chosen string (e.g. document URI) for unregistration.
    /// If the workspace is already loaded, the callback fires on the next Poll().
    void RegisterWorkspaceLoadedCallback(const std::string &key, WorkspaceLoadedCallback callback);

    /// Unregister a previously registered workspace-loaded callback.
    void UnregisterWorkspaceLoadedCallback(const std::string &key);

    /// Dispatch any pending callbacks on the UI thread.
    /// Must be called periodically (e.g., on a wxTimer at ~100ms interval).
    /// This does no I/O — just swaps a pre-built list and invokes callbacks.
    void Poll();

  private:
    void SendRequest(const std::string &method, const nlohmann::json &params);
    void SendNotification(const std::string &method, const nlohmann::json &params);
    void HandleMessage(const nlohmann::json &message);
    void HandleResponse(int id, const nlohmann::json &result);
    void HandleNotification(const std::string &method, const nlohmann::json &params);

    /// Handle a server→client request (has both id and method).
    void HandleServerRequest(int id, const std::string &method, const nlohmann::json &params);

    /// Enqueue a JSON-RPC message to be sent by the I/O thread.
    void Send(const nlohmann::json &message);

    /// The I/O thread run loop.
    void IoThreadMain();

    /// Run the initialize handshake (called on I/O thread).
    bool RunInitHandshake(const std::string &workspaceRoot);

    CProcess m_process;
    JsonRpc m_framing; // Only accessed by I/O thread after startup

    int m_nextRequestId = 1; // Protected by m_mtx
    std::atomic<bool> m_bReady{false};
    std::atomic<bool> m_bStartFailed{false};
    std::atomic<bool> m_bWorkspaceLoaded{false};

    // ----- Threading -----
    std::thread m_ioThread;
    std::atomic<bool> m_bStopRequested{false};
    mutable std::mutex m_mtx;
    std::condition_variable m_cvReady; // Signalled when init completes (for blocking Start)

    /// Outbound message queue — UI thread enqueues, I/O thread drains.
    std::vector<std::string> m_outQueue;

    /// Pending callbacks ready to fire — I/O thread produces, UI thread consumes via Poll().
    std::vector<std::function<void()>> m_pendingCallbacks;

    /// Pending request callbacks, keyed by request ID. Protected by m_mtx.
    std::map<int, CompletionCallback> m_completionCallbacks;
    std::map<int, SignatureHelpCallback> m_signatureHelpCallbacks;
    std::map<int, HoverCallback> m_hoverCallbacks;
    std::map<int, DefinitionCallback> m_definitionCallbacks;

    /// Per-URI diagnostics callbacks (server-initiated notifications). Protected by m_mtx.
    std::map<std::string, DiagnosticsCallback> m_diagnosticsCallbacks;

    /// Workspace-loaded callbacks, keyed by caller string. Protected by m_mtx.
    std::map<std::string, WorkspaceLoadedCallback> m_workspaceLoadedCallbacks;

    /// Active $/progress tokens (workspace loading). Protected by m_mtx.
    std::set<std::string> m_activeProgressTokens;

    /// Document version tracking for didChange. Protected by m_mtx.
    std::map<std::string, int> m_documentVersions;
};

} // namespace lsp
