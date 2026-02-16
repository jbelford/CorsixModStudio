#include <gtest/gtest.h>
#include "core/Callbacks.h"
#include <cstring>
#include <string>

namespace {

struct CallbackCapture {
    std::string lastMessage;
    void* lastTag;
    int callCount;
};

void TestCallback(const char* sMessage, void* pToken) {
    auto* capture = static_cast<CallbackCapture*>(pToken);
    capture->lastMessage = sMessage;
    capture->lastTag = pToken;
    capture->callCount++;
}

} // namespace

TEST(CallbacksTest, NullCallbackDoesNotCrash) {
    // Passing null callback should be a no-op
    CallCallback(nullptr, nullptr, "this should not crash %d", 42);
}

TEST(CallbacksTest, CallbackReceivesFormattedMessage) {
    CallbackCapture capture = {"", nullptr, 0};
    CallCallback(TestCallback, &capture, "hello %s", "world");
    EXPECT_EQ(capture.lastMessage, "hello world");
    EXPECT_EQ(capture.callCount, 1);
}

TEST(CallbacksTest, CallbackReceivesTagPointer) {
    CallbackCapture capture = {"", nullptr, 0};
    CallCallback(TestCallback, &capture, "test");
    EXPECT_EQ(capture.lastTag, &capture);
}

TEST(CallbacksTest, PrintfStyleMultipleArgs) {
    CallbackCapture capture = {"", nullptr, 0};
    CallCallback(TestCallback, &capture, "values: %d, %s, %.1f", 42, "abc", 3.14);
    EXPECT_EQ(capture.lastMessage, "values: 42, abc, 3.1");
}

TEST(CallbacksTest, MultipleCallbackInvocations) {
    CallbackCapture capture = {"", nullptr, 0};
    CallCallback(TestCallback, &capture, "first");
    CallCallback(TestCallback, &capture, "second");
    EXPECT_EQ(capture.lastMessage, "second");
    EXPECT_EQ(capture.callCount, 2);
}

TEST(CallbacksTest, EmptyFormatString) {
    CallbackCapture capture = {"", nullptr, 0};
    CallCallback(TestCallback, &capture, "");
    EXPECT_EQ(capture.lastMessage, "");
    EXPECT_EQ(capture.callCount, 1);
}
