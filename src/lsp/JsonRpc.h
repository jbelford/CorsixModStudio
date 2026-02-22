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

#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <utility>

namespace lsp
{

/// JSON-RPC 2.0 framing layer for the Language Server Protocol.
///
/// Handles Content-Length header encoding (outgoing) and streaming
/// decode (incoming). No dependency on LSP message types — pure framing.
class JsonRpc
{
  public:
    /// Encode a JSON message with Content-Length header.
    /// Returns the full wire-format string: "Content-Length: N\r\n\r\n{...}"
    static std::string Encode(const nlohmann::json &message);

    /// Feed raw bytes from the transport into the internal buffer.
    void Feed(const char *data, size_t length);
    void Feed(const std::string &data);

    /// Returns true if at least one complete message is buffered.
    [[nodiscard]] bool HasMessage() const;

    /// Try to extract the next complete JSON-RPC message.
    /// Returns std::nullopt if no complete message is available yet.
    [[nodiscard]] std::optional<nlohmann::json> TryParse();

    /// Clear any buffered partial data.
    void Reset() { m_buffer.clear(); }

  private:
    std::string m_buffer;

    /// Locate the body in the buffer. Returns (bodyStart, contentLength)
    /// or nullopt if the message is incomplete.
    static std::optional<std::pair<size_t, size_t>> FindBody(const std::string &buffer);
};

} // namespace lsp
