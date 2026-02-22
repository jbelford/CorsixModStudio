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
#include <vector>

namespace lsp
{

// ---------------------------------------------------------------------------
// Core LSP types
// ---------------------------------------------------------------------------

struct Position
{
    int line = 0;
    int character = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Position, line, character)

struct Range
{
    Position start;
    Position end;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Range, start, end)

struct TextDocumentIdentifier
{
    std::string uri;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextDocumentIdentifier, uri)

struct TextDocumentItem
{
    std::string uri;
    std::string languageId;
    int version = 0;
    std::string text;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextDocumentItem, uri, languageId, version, text)

struct TextDocumentPositionParams
{
    TextDocumentIdentifier textDocument;
    Position position;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextDocumentPositionParams, textDocument, position)

// ---------------------------------------------------------------------------
// Diagnostics (server → client)
// ---------------------------------------------------------------------------

enum class DiagnosticSeverity
{
    Error = 1,
    Warning = 2,
    Information = 3,
    Hint = 4
};

struct Diagnostic
{
    Range range;
    DiagnosticSeverity severity = DiagnosticSeverity::Error;
    std::string message;
    std::string source;
    std::optional<std::string> code;
};

inline void to_json(nlohmann::json &j, const Diagnostic &d)
{
    j = {{"range", d.range}, {"severity", static_cast<int>(d.severity)}, {"message", d.message}, {"source", d.source}};
    if (d.code)
    {
        j["code"] = *d.code;
    }
}

inline void from_json(const nlohmann::json &j, Diagnostic &d)
{
    j.at("range").get_to(d.range);
    d.severity = static_cast<DiagnosticSeverity>(j.at("severity").get<int>());
    j.at("message").get_to(d.message);
    if (j.contains("source"))
    {
        j.at("source").get_to(d.source);
    }
    if (j.contains("code"))
    {
        if (j["code"].is_string())
        {
            d.code = j["code"].get<std::string>();
        }
        else if (j["code"].is_number())
        {
            d.code = std::to_string(j["code"].get<int>());
        }
    }
}

struct PublishDiagnosticsParams
{
    std::string uri;
    std::vector<Diagnostic> diagnostics;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PublishDiagnosticsParams, uri, diagnostics)

// ---------------------------------------------------------------------------
// textDocument/didOpen
// ---------------------------------------------------------------------------

struct DidOpenTextDocumentParams
{
    TextDocumentItem textDocument;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DidOpenTextDocumentParams, textDocument)

// ---------------------------------------------------------------------------
// textDocument/didChange (full document sync)
// ---------------------------------------------------------------------------

struct TextDocumentContentChangeEvent
{
    std::string text;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextDocumentContentChangeEvent, text)

struct VersionedTextDocumentIdentifier
{
    std::string uri;
    int version = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(VersionedTextDocumentIdentifier, uri, version)

struct DidChangeTextDocumentParams
{
    VersionedTextDocumentIdentifier textDocument;
    std::vector<TextDocumentContentChangeEvent> contentChanges;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DidChangeTextDocumentParams, textDocument, contentChanges)

// ---------------------------------------------------------------------------
// textDocument/didClose
// ---------------------------------------------------------------------------

struct DidCloseTextDocumentParams
{
    TextDocumentIdentifier textDocument;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DidCloseTextDocumentParams, textDocument)

// ---------------------------------------------------------------------------
// textDocument/completion
// ---------------------------------------------------------------------------

enum class CompletionItemKind
{
    Text = 1,
    Method = 2,
    Function = 3,
    Constructor = 4,
    Field = 5,
    Variable = 6,
    Class = 7,
    Interface = 8,
    Module = 9,
    Property = 10,
    Unit = 11,
    Value = 12,
    Enum = 13,
    Keyword = 14,
    Snippet = 15,
    Color = 16,
    File = 17,
    Reference = 18,
    Folder = 19,
    EnumMember = 20,
    Constant = 21,
    Struct = 22,
    Event = 23,
    Operator = 24,
    TypeParameter = 25
};

struct CompletionItem
{
    std::string label;
    CompletionItemKind kind = CompletionItemKind::Text;
    std::optional<std::string> detail;
    std::optional<std::string> documentation;
    std::optional<std::string> insertText;
};

inline void to_json(nlohmann::json &j, const CompletionItem &c)
{
    j = {{"label", c.label}, {"kind", static_cast<int>(c.kind)}};
    if (c.detail)
    {
        j["detail"] = *c.detail;
    }
    if (c.documentation)
    {
        j["documentation"] = *c.documentation;
    }
    if (c.insertText)
    {
        j["insertText"] = *c.insertText;
    }
}

inline void from_json(const nlohmann::json &j, CompletionItem &c)
{
    j.at("label").get_to(c.label);
    if (j.contains("kind"))
    {
        c.kind = static_cast<CompletionItemKind>(j.at("kind").get<int>());
    }
    if (j.contains("detail"))
    {
        if (j["detail"].is_string())
        {
            c.detail = j["detail"].get<std::string>();
        }
    }
    if (j.contains("documentation"))
    {
        if (j["documentation"].is_string())
        {
            c.documentation = j["documentation"].get<std::string>();
        }
        else if (j["documentation"].is_object() && j["documentation"].contains("value"))
        {
            c.documentation = j["documentation"]["value"].get<std::string>();
        }
    }
    if (j.contains("insertText"))
    {
        c.insertText = j["insertText"].get<std::string>();
    }
}

struct CompletionList
{
    bool isIncomplete = false;
    std::vector<CompletionItem> items;
};

inline void to_json(nlohmann::json &j, const CompletionList &c)
{
    j = {{"isIncomplete", c.isIncomplete}, {"items", c.items}};
}

inline void from_json(const nlohmann::json &j, CompletionList &c)
{
    if (j.is_array())
    {
        // Some servers return CompletionItem[] directly instead of CompletionList
        c.isIncomplete = false;
        c.items = j.get<std::vector<CompletionItem>>();
    }
    else
    {
        j.at("isIncomplete").get_to(c.isIncomplete);
        j.at("items").get_to(c.items);
    }
}

struct CompletionParams
{
    TextDocumentIdentifier textDocument;
    Position position;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CompletionParams, textDocument, position)

// ---------------------------------------------------------------------------
// textDocument/signatureHelp
// ---------------------------------------------------------------------------

struct ParameterInformation
{
    std::string label;
    std::optional<std::string> documentation;
};

inline void to_json(nlohmann::json &j, const ParameterInformation &p)
{
    j = {{"label", p.label}};
    if (p.documentation)
    {
        j["documentation"] = *p.documentation;
    }
}

inline void from_json(const nlohmann::json &j, ParameterInformation &p)
{
    if (j["label"].is_string())
    {
        j["label"].get_to(p.label);
    }
    else if (j["label"].is_array())
    {
        // Label can be [start, end] offsets — convert to string representation
        p.label = j["label"].dump();
    }
    if (j.contains("documentation"))
    {
        if (j["documentation"].is_string())
        {
            p.documentation = j["documentation"].get<std::string>();
        }
        else if (j["documentation"].is_object() && j["documentation"].contains("value"))
        {
            p.documentation = j["documentation"]["value"].get<std::string>();
        }
    }
}

struct SignatureInformation
{
    std::string label;
    std::optional<std::string> documentation;
    std::vector<ParameterInformation> parameters;
};

inline void to_json(nlohmann::json &j, const SignatureInformation &s)
{
    j = {{"label", s.label}, {"parameters", s.parameters}};
    if (s.documentation)
    {
        j["documentation"] = *s.documentation;
    }
}

inline void from_json(const nlohmann::json &j, SignatureInformation &s)
{
    j.at("label").get_to(s.label);
    if (j.contains("documentation"))
    {
        if (j["documentation"].is_string())
        {
            s.documentation = j["documentation"].get<std::string>();
        }
        else if (j["documentation"].is_object() && j["documentation"].contains("value"))
        {
            s.documentation = j["documentation"]["value"].get<std::string>();
        }
    }
    if (j.contains("parameters"))
    {
        j["parameters"].get_to(s.parameters);
    }
}

struct SignatureHelp
{
    std::vector<SignatureInformation> signatures;
    int activeSignature = 0;
    int activeParameter = 0;
};

inline void to_json(nlohmann::json &j, const SignatureHelp &s)
{
    j = {{"signatures", s.signatures}, {"activeSignature", s.activeSignature}, {"activeParameter", s.activeParameter}};
}

inline void from_json(const nlohmann::json &j, SignatureHelp &s)
{
    j.at("signatures").get_to(s.signatures);
    if (j.contains("activeSignature"))
    {
        j["activeSignature"].get_to(s.activeSignature);
    }
    if (j.contains("activeParameter"))
    {
        j["activeParameter"].get_to(s.activeParameter);
    }
}

struct SignatureHelpParams
{
    TextDocumentIdentifier textDocument;
    Position position;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SignatureHelpParams, textDocument, position)

// ---------------------------------------------------------------------------
// textDocument/hover
// ---------------------------------------------------------------------------

/// Markup content returned by the server (e.g. documentation).
struct MarkupContent
{
    std::string kind; // "plaintext" or "markdown"
    std::string value;
};

inline void from_json(const nlohmann::json &j, MarkupContent &m)
{
    if (j.contains("kind"))
    {
        j["kind"].get_to(m.kind);
    }
    if (j.contains("value"))
    {
        j["value"].get_to(m.value);
    }
}

inline void to_json(nlohmann::json &j, const MarkupContent &m) { j = {{"kind", m.kind}, {"value", m.value}}; }

struct HoverResult
{
    std::string contents; // Flattened plain-text contents
    std::optional<Range> range;
};

inline void from_json(const nlohmann::json &j, HoverResult &h)
{
    // contents can be: string, MarkupContent, or array of (string | MarkupContent)
    if (j.contains("contents"))
    {
        const auto &c = j["contents"];
        if (c.is_string())
        {
            h.contents = c.get<std::string>();
        }
        else if (c.is_object())
        {
            // MarkupContent { kind, value }
            if (c.contains("value"))
            {
                h.contents = c["value"].get<std::string>();
            }
        }
        else if (c.is_array())
        {
            // Array of string | { language, value }
            std::string combined;
            for (const auto &item : c)
            {
                if (!combined.empty())
                {
                    combined += "\n";
                }
                if (item.is_string())
                {
                    combined += item.get<std::string>();
                }
                else if (item.is_object() && item.contains("value"))
                {
                    combined += item["value"].get<std::string>();
                }
            }
            h.contents = combined;
        }
    }
    if (j.contains("range"))
    {
        h.range = j["range"].get<Range>();
    }
}

inline void to_json(nlohmann::json &j, const HoverResult &h)
{
    j = {{"contents", h.contents}};
    if (h.range)
    {
        j["range"] = *h.range;
    }
}

using HoverParams = TextDocumentPositionParams;

} // namespace lsp
