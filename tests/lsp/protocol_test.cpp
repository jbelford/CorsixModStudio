/*
LSP Client Library — Tests
Copyright (C) 2026 CorsixModStudio Contributors
LGPL v2.1 — see LICENSE for details.
*/

#include <gtest/gtest.h>
#include <lsp/Protocol.h>

using namespace lsp;

// ---------------------------------------------------------------------------
// Position / Range
// ---------------------------------------------------------------------------

TEST(Protocol, PositionRoundTrip)
{
    Position pos{10, 25};
    nlohmann::json j = pos;
    auto parsed = j.get<Position>();
    EXPECT_EQ(parsed.line, 10);
    EXPECT_EQ(parsed.character, 25);
}

TEST(Protocol, RangeRoundTrip)
{
    Range r{{1, 0}, {1, 15}};
    nlohmann::json j = r;
    auto parsed = j.get<Range>();
    EXPECT_EQ(parsed.start.line, 1);
    EXPECT_EQ(parsed.start.character, 0);
    EXPECT_EQ(parsed.end.line, 1);
    EXPECT_EQ(parsed.end.character, 15);
}

// ---------------------------------------------------------------------------
// TextDocumentItem
// ---------------------------------------------------------------------------

TEST(Protocol, TextDocumentItemRoundTrip)
{
    TextDocumentItem item;
    item.uri = "file:///c:/test.lua";
    item.languageId = "lua";
    item.version = 3;
    item.text = "print('hello')";

    nlohmann::json j = item;
    auto parsed = j.get<TextDocumentItem>();
    EXPECT_EQ(parsed.uri, "file:///c:/test.lua");
    EXPECT_EQ(parsed.languageId, "lua");
    EXPECT_EQ(parsed.version, 3);
    EXPECT_EQ(parsed.text, "print('hello')");
}

// ---------------------------------------------------------------------------
// Diagnostics
// ---------------------------------------------------------------------------

TEST(Protocol, DiagnosticRoundTrip)
{
    Diagnostic d;
    d.range = {{5, 0}, {5, 10}};
    d.severity = DiagnosticSeverity::Warning;
    d.message = "Unused variable 'x'";
    d.source = "Lua Diagnostics.";
    d.code = "unused-local";

    nlohmann::json j = d;
    auto parsed = j.get<Diagnostic>();
    EXPECT_EQ(parsed.range.start.line, 5);
    EXPECT_EQ(parsed.severity, DiagnosticSeverity::Warning);
    EXPECT_EQ(parsed.message, "Unused variable 'x'");
    EXPECT_EQ(parsed.source, "Lua Diagnostics.");
    ASSERT_TRUE(parsed.code.has_value());
    EXPECT_EQ(*parsed.code, "unused-local");
}

TEST(Protocol, DiagnosticWithNumericCode)
{
    nlohmann::json j = {{"range", {{"start", {{"line", 0}, {"character", 0}}}, {"end", {{"line", 0}, {"character", 5}}}}},
                        {"severity", 1},
                        {"message", "error"},
                        {"source", "lua"},
                        {"code", 42}};

    auto d = j.get<Diagnostic>();
    ASSERT_TRUE(d.code.has_value());
    EXPECT_EQ(*d.code, "42");
}

TEST(Protocol, DiagnosticWithoutOptionalFields)
{
    nlohmann::json j = {{"range", {{"start", {{"line", 0}, {"character", 0}}}, {"end", {{"line", 0}, {"character", 5}}}}},
                        {"severity", 1},
                        {"message", "error"}};

    auto d = j.get<Diagnostic>();
    EXPECT_FALSE(d.code.has_value());
    EXPECT_TRUE(d.source.empty());
}

TEST(Protocol, PublishDiagnosticsRoundTrip)
{
    PublishDiagnosticsParams params;
    params.uri = "file:///c:/mod/scripts/main.scar";
    params.diagnostics.push_back({{{1, 0}, {1, 5}}, DiagnosticSeverity::Error, "Syntax error", "luaLS", "syntax"});

    nlohmann::json j = params;
    auto parsed = j.get<PublishDiagnosticsParams>();
    EXPECT_EQ(parsed.uri, "file:///c:/mod/scripts/main.scar");
    ASSERT_EQ(parsed.diagnostics.size(), 1u);
    EXPECT_EQ(parsed.diagnostics[0].message, "Syntax error");
}

// ---------------------------------------------------------------------------
// Completion
// ---------------------------------------------------------------------------

TEST(Protocol, CompletionItemRoundTrip)
{
    CompletionItem item;
    item.label = "SGroup_Create";
    item.kind = CompletionItemKind::Function;
    item.detail = "fun(name: string): SGroupID";
    item.documentation = "Creates a new squad group";

    nlohmann::json j = item;
    auto parsed = j.get<CompletionItem>();
    EXPECT_EQ(parsed.label, "SGroup_Create");
    EXPECT_EQ(parsed.kind, CompletionItemKind::Function);
    ASSERT_TRUE(parsed.detail.has_value());
    EXPECT_EQ(*parsed.detail, "fun(name: string): SGroupID");
}

TEST(Protocol, CompletionItemWithMarkupDocumentation)
{
    nlohmann::json j = {{"label", "test"},
                        {"kind", 3},
                        {"documentation", {{"kind", "markdown"}, {"value", "**bold** text"}}}};

    auto item = j.get<CompletionItem>();
    ASSERT_TRUE(item.documentation.has_value());
    EXPECT_EQ(*item.documentation, "**bold** text");
}

TEST(Protocol, CompletionListRoundTrip)
{
    CompletionList list;
    list.isIncomplete = true;
    list.items.push_back({"func1", CompletionItemKind::Function, {}, {}, {}});
    list.items.push_back({"var1", CompletionItemKind::Variable, {}, {}, {}});

    nlohmann::json j = list;
    auto parsed = j.get<CompletionList>();
    EXPECT_TRUE(parsed.isIncomplete);
    ASSERT_EQ(parsed.items.size(), 2u);
    EXPECT_EQ(parsed.items[0].label, "func1");
    EXPECT_EQ(parsed.items[1].kind, CompletionItemKind::Variable);
}

TEST(Protocol, CompletionListFromArray)
{
    // Some servers return CompletionItem[] directly
    nlohmann::json j = nlohmann::json::array({{{"label", "a"}, {"kind", 1}}, {{"label", "b"}, {"kind", 3}}});

    auto list = j.get<CompletionList>();
    EXPECT_FALSE(list.isIncomplete);
    ASSERT_EQ(list.items.size(), 2u);
}

// ---------------------------------------------------------------------------
// SignatureHelp
// ---------------------------------------------------------------------------

TEST(Protocol, SignatureHelpRoundTrip)
{
    SignatureHelp help;
    SignatureInformation sig;
    sig.label = "SGroup_Create(name: string): SGroupID";
    sig.documentation = "Creates a new squad group";
    sig.parameters.push_back({"name: string", "The group name"});
    help.signatures.push_back(sig);
    help.activeSignature = 0;
    help.activeParameter = 0;

    nlohmann::json j = help;
    auto parsed = j.get<SignatureHelp>();
    ASSERT_EQ(parsed.signatures.size(), 1u);
    EXPECT_EQ(parsed.signatures[0].label, "SGroup_Create(name: string): SGroupID");
    ASSERT_EQ(parsed.signatures[0].parameters.size(), 1u);
    EXPECT_EQ(parsed.signatures[0].parameters[0].label, "name: string");
}

// ---------------------------------------------------------------------------
// DidOpen / DidChange / DidClose
// ---------------------------------------------------------------------------

TEST(Protocol, DidOpenRoundTrip)
{
    DidOpenTextDocumentParams params;
    params.textDocument.uri = "file:///test.lua";
    params.textDocument.languageId = "lua";
    params.textDocument.version = 1;
    params.textDocument.text = "local x = 1";

    nlohmann::json j = params;
    auto parsed = j.get<DidOpenTextDocumentParams>();
    EXPECT_EQ(parsed.textDocument.uri, "file:///test.lua");
    EXPECT_EQ(parsed.textDocument.text, "local x = 1");
}

TEST(Protocol, DidChangeRoundTrip)
{
    DidChangeTextDocumentParams params;
    params.textDocument.uri = "file:///test.lua";
    params.textDocument.version = 2;
    params.contentChanges.push_back({"local x = 2"});

    nlohmann::json j = params;
    auto parsed = j.get<DidChangeTextDocumentParams>();
    EXPECT_EQ(parsed.textDocument.version, 2);
    ASSERT_EQ(parsed.contentChanges.size(), 1u);
    EXPECT_EQ(parsed.contentChanges[0].text, "local x = 2");
}

TEST(Protocol, DidCloseRoundTrip)
{
    DidCloseTextDocumentParams params;
    params.textDocument.uri = "file:///test.lua";

    nlohmann::json j = params;
    auto parsed = j.get<DidCloseTextDocumentParams>();
    EXPECT_EQ(parsed.textDocument.uri, "file:///test.lua");
}
