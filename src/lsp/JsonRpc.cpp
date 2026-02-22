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

#include "lsp/JsonRpc.h"
#include <charconv>
#include <cstring>
#include <stdexcept>

namespace lsp
{

// ---------------------------------------------------------------------------
// Encoding
// ---------------------------------------------------------------------------

std::string JsonRpc::Encode(const nlohmann::json &message)
{
    std::string body = message.dump();
    std::string header = "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n";
    return header + body;
}

// ---------------------------------------------------------------------------
// Decoding (streaming)
// ---------------------------------------------------------------------------

void JsonRpc::Feed(const char *data, size_t length) { m_buffer.append(data, length); }

void JsonRpc::Feed(const std::string &data) { m_buffer.append(data); }

bool JsonRpc::HasMessage() const { return FindBody(m_buffer).has_value(); }

std::optional<nlohmann::json> JsonRpc::TryParse()
{
    auto body = FindBody(m_buffer);
    if (!body)
    {
        return std::nullopt;
    }

    auto [headerEnd, contentLength] = *body;
    size_t messageEnd = headerEnd + contentLength;

    std::string jsonStr = m_buffer.substr(headerEnd, contentLength);
    m_buffer.erase(0, messageEnd);

    return nlohmann::json::parse(jsonStr);
}

std::optional<std::pair<size_t, size_t>> JsonRpc::FindBody(const std::string &buffer)
{
    // Look for "Content-Length: <digits>\r\n\r\n"
    static constexpr const char *kHeaderPrefix = "Content-Length: ";
    static constexpr size_t kHeaderPrefixLen = 16;
    static constexpr const char *kHeaderEnd = "\r\n\r\n";

    auto prefixPos = buffer.find(kHeaderPrefix);
    if (prefixPos == std::string::npos)
    {
        return std::nullopt;
    }

    auto lineEnd = buffer.find("\r\n", prefixPos + kHeaderPrefixLen);
    if (lineEnd == std::string::npos)
    {
        return std::nullopt;
    }

    // Parse the content length value
    const char *numStart = buffer.data() + prefixPos + kHeaderPrefixLen;
    const char *numEnd = buffer.data() + lineEnd;
    size_t contentLength = 0;
    auto [ptr, ec] = std::from_chars(numStart, numEnd, contentLength);
    if (ec != std::errc{} || ptr != numEnd)
    {
        return std::nullopt;
    }

    // Find the header terminator (\r\n\r\n)
    auto headerEndPos = buffer.find(kHeaderEnd, prefixPos);
    if (headerEndPos == std::string::npos)
    {
        return std::nullopt;
    }

    size_t bodyStart = headerEndPos + 4; // skip \r\n\r\n

    // Check if we have the full body
    if (buffer.size() < bodyStart + contentLength)
    {
        return std::nullopt;
    }

    return std::make_pair(bodyStart, contentLength);
}

} // namespace lsp
