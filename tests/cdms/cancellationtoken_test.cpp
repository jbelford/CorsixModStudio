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

#include "async/CancellationToken.h"
#include <gtest/gtest.h>
#include <thread>

TEST(CCancellationTokenTest, InitiallyNotCancelled)
{
    CCancellationToken token;
    EXPECT_FALSE(token.IsCancelled());
}

TEST(CCancellationTokenTest, CancelSetsFlagTrue)
{
    CCancellationToken token;
    token.Cancel();
    EXPECT_TRUE(token.IsCancelled());
}

TEST(CCancellationTokenTest, ResetClearsCancellation)
{
    CCancellationToken token;
    token.Cancel();
    EXPECT_TRUE(token.IsCancelled());
    token.Reset();
    EXPECT_FALSE(token.IsCancelled());
}

TEST(CCancellationTokenTest, CancelFromAnotherThread)
{
    CCancellationToken token;

    std::thread t([&token]() { token.Cancel(); });
    t.join();

    EXPECT_TRUE(token.IsCancelled());
}

TEST(CCancellationTokenTest, MultipleCancelsAreIdempotent)
{
    CCancellationToken token;
    token.Cancel();
    token.Cancel();
    token.Cancel();
    EXPECT_TRUE(token.IsCancelled());
}
