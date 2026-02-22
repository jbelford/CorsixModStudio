/*
LSP Client Library — Tests
Copyright (C) 2026 CorsixModStudio Contributors
LGPL v2.1 — see LICENSE for details.
*/

#include <gtest/gtest.h>
#include <lsp/JsonRpc.h>

using namespace lsp;

// ---------------------------------------------------------------------------
// Encoding
// ---------------------------------------------------------------------------

TEST(JsonRpc, EncodeSimpleMessage)
{
    nlohmann::json msg = {{"jsonrpc", "2.0"}, {"method", "test"}};
    std::string encoded = JsonRpc::Encode(msg);

    std::string body = msg.dump();
    std::string expected = "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
    EXPECT_EQ(encoded, expected);
}

TEST(JsonRpc, EncodeEmptyObject)
{
    nlohmann::json msg = nlohmann::json::object();
    std::string encoded = JsonRpc::Encode(msg);
    EXPECT_EQ(encoded, "Content-Length: 2\r\n\r\n{}");
}

// ---------------------------------------------------------------------------
// Decoding — complete messages
// ---------------------------------------------------------------------------

TEST(JsonRpc, DecodeCompleteMessage)
{
    JsonRpc framing;
    nlohmann::json original = {{"jsonrpc", "2.0"}, {"method", "test"}, {"id", 1}};
    std::string wire = JsonRpc::Encode(original);

    framing.Feed(wire);
    ASSERT_TRUE(framing.HasMessage());

    auto parsed = framing.TryParse();
    ASSERT_TRUE(parsed.has_value());
    EXPECT_EQ(*parsed, original);
    EXPECT_FALSE(framing.HasMessage());
}

TEST(JsonRpc, DecodeMultipleMessages)
{
    JsonRpc framing;
    nlohmann::json msg1 = {{"id", 1}};
    nlohmann::json msg2 = {{"id", 2}};

    framing.Feed(JsonRpc::Encode(msg1) + JsonRpc::Encode(msg2));

    auto p1 = framing.TryParse();
    ASSERT_TRUE(p1.has_value());
    EXPECT_EQ((*p1)["id"], 1);

    auto p2 = framing.TryParse();
    ASSERT_TRUE(p2.has_value());
    EXPECT_EQ((*p2)["id"], 2);

    EXPECT_FALSE(framing.HasMessage());
}

// ---------------------------------------------------------------------------
// Decoding — partial/streaming
// ---------------------------------------------------------------------------

TEST(JsonRpc, DecodePartialHeader)
{
    JsonRpc framing;
    framing.Feed("Content-Len");
    EXPECT_FALSE(framing.HasMessage());
    EXPECT_FALSE(framing.TryParse().has_value());
}

TEST(JsonRpc, DecodePartialBody)
{
    JsonRpc framing;
    nlohmann::json msg = {{"key", "value"}};
    std::string wire = JsonRpc::Encode(msg);

    // Feed header + partial body
    framing.Feed(wire.substr(0, wire.size() - 3));
    EXPECT_FALSE(framing.HasMessage());

    // Feed remaining bytes
    framing.Feed(wire.substr(wire.size() - 3));
    ASSERT_TRUE(framing.HasMessage());

    auto parsed = framing.TryParse();
    ASSERT_TRUE(parsed.has_value());
    EXPECT_EQ(*parsed, msg);
}

TEST(JsonRpc, DecodeByteByByte)
{
    JsonRpc framing;
    nlohmann::json msg = {{"method", "textDocument/didOpen"}};
    std::string wire = JsonRpc::Encode(msg);

    for (size_t i = 0; i < wire.size() - 1; ++i)
    {
        framing.Feed(wire.data() + i, 1);
        EXPECT_FALSE(framing.HasMessage()) << "Unexpected complete message at byte " << i;
    }

    framing.Feed(wire.data() + wire.size() - 1, 1);
    ASSERT_TRUE(framing.HasMessage());

    auto parsed = framing.TryParse();
    ASSERT_TRUE(parsed.has_value());
    EXPECT_EQ(*parsed, msg);
}

// ---------------------------------------------------------------------------
// Round-trip
// ---------------------------------------------------------------------------

TEST(JsonRpc, RoundTripLargeMessage)
{
    JsonRpc framing;
    nlohmann::json msg;
    msg["method"] = "textDocument/publishDiagnostics";
    msg["params"]["uri"] = "file:///c:/projects/test.scar";
    msg["params"]["diagnostics"] = nlohmann::json::array();
    for (int i = 0; i < 100; ++i)
    {
        msg["params"]["diagnostics"].push_back(
            {{"range", {{"start", {{"line", i}, {"character", 0}}}, {"end", {{"line", i}, {"character", 10}}}}},
             {"message", "Error number " + std::to_string(i)},
             {"severity", 1}});
    }

    framing.Feed(JsonRpc::Encode(msg));
    auto parsed = framing.TryParse();
    ASSERT_TRUE(parsed.has_value());
    EXPECT_EQ(*parsed, msg);
}

TEST(JsonRpc, ResetClearsBuffer)
{
    JsonRpc framing;
    framing.Feed("Content-Length: 999\r\n\r\n{partial...");
    EXPECT_FALSE(framing.HasMessage());

    framing.Reset();

    // After reset, a new message should parse cleanly
    nlohmann::json msg = {{"after", "reset"}};
    framing.Feed(JsonRpc::Encode(msg));
    auto parsed = framing.TryParse();
    ASSERT_TRUE(parsed.has_value());
    EXPECT_EQ(*parsed, msg);
}
