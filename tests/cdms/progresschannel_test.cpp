/*
    This file is part of Corsix's Mod Studio.

    Corsix's Mod Studio is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Corsix's Mod Studio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Corsix's Mod Studio; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "async/CProgressChannel.h"
#include <gtest/gtest.h>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

TEST(CProgressChannelTest, ReportDeliversMessage)
{
    std::string received;
    CProgressChannel channel([&received](const std::string &msg) { received = msg; });

    channel.Report("Loading...");
    EXPECT_EQ(received, "Loading...");
}

TEST(CProgressChannelTest, ReportDeliversMultipleMessages)
{
    std::vector<std::string> received;
    CProgressChannel channel(
        [&received](const std::string &msg) { received.push_back(msg); });

    channel.Report("Step 1");
    channel.Report("Step 2");
    channel.Report("Step 3");

    ASSERT_EQ(received.size(), 3u);
    EXPECT_EQ(received[0], "Step 1");
    EXPECT_EQ(received[1], "Step 2");
    EXPECT_EQ(received[2], "Step 3");
}

TEST(CProgressChannelTest, NullCallbackDoesNotCrash)
{
    CProgressChannel channel(nullptr);
    EXPECT_NO_THROW(channel.Report("ignored"));
}

TEST(CProgressChannelTest, RainmanCallbackBridge)
{
    std::string received;
    CProgressChannel channel([&received](const std::string &msg) { received = msg; });

    CProgressChannel::RainmanCallback("hello from rainman", &channel);
    EXPECT_EQ(received, "hello from rainman");
}

TEST(CProgressChannelTest, RainmanCallbackNullMessage)
{
    std::string received;
    CProgressChannel channel([&received](const std::string &msg) { received = msg; });

    CProgressChannel::RainmanCallback(nullptr, &channel);
    EXPECT_TRUE(received.empty());
}

TEST(CProgressChannelTest, RainmanCallbackNullTag)
{
    EXPECT_NO_THROW(CProgressChannel::RainmanCallback("msg", nullptr));
}

TEST(CProgressChannelTest, ReportFromBackgroundThread)
{
    std::mutex mtx;
    std::vector<std::string> received;
    CProgressChannel channel([&](const std::string &msg) {
        std::lock_guard<std::mutex> lock(mtx);
        received.push_back(msg);
    });

    std::thread t([&channel]() {
        channel.Report("from thread 1");
        channel.Report("from thread 2");
    });
    t.join();

    ASSERT_EQ(received.size(), 2u);
    EXPECT_EQ(received[0], "from thread 1");
    EXPECT_EQ(received[1], "from thread 2");
}
