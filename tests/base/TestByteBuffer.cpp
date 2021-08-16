/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libtest/TestCase.h>
#include <base/ByteBuffer.h>

TEST_CASE(equality_operator)
{
    ByteBuffer a = ByteBuffer::copy("Hello, world", 7);
    ByteBuffer b = ByteBuffer::copy("Hello, friend", 7);

    ByteBuffer c = ByteBuffer::copy("asdf", 4);
    ByteBuffer d;
    EXPECT_EQ(a == a, true);
    EXPECT_EQ(a == b, true);
    EXPECT_EQ(a == c, false);
    EXPECT_EQ(a == d, false);
    EXPECT_EQ(b == a, true);
    EXPECT_EQ(b == b, true);
    EXPECT_EQ(b == c, false);
    EXPECT_EQ(b == d, false);
    EXPECT_EQ(c == a, false);
    EXPECT_EQ(c == b, false);
    EXPECT_EQ(c == c, true);
    EXPECT_EQ(c == d, false);
    EXPECT_EQ(d == a, false);
    EXPECT_EQ(d == b, false);
    EXPECT_EQ(d == c, false);
    EXPECT_EQ(d == d, true);
}

#define COMPILE_NEGATIVE_TESTS 0
#if COMPILE_NEGATIVE_TESTS
TEST_CASE(negative_operator_lt)
{
    ByteBuffer a = ByteBuffer::copy("Hello, world", 10);
    ByteBuffer b = ByteBuffer::copy("Hello, friend", 10);
    [[maybe_unused]] auto res = a < b;
}
#endif 
