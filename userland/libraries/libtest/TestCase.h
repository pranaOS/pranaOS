/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libtest/Macros.h> 
#include <base/Function.h>
#include <base/NonnullRefPtr.h>
#include <base/RefCounted.h>
#include <base/String.h>

namespace Test {

using TestFunction = Function<void()>;

class TestCase : public RefCounted<TestCase> {
public:
    TestCase(const String& name, TestFunction&& fn, bool is_benchmark)
        : m_name(name)
        , m_function(move(fn))
        , m_is_benchmark(is_benchmark)
    {
    }

    bool is_benchmark() const { return m_is_benchmark; }
    const String& name() const { return m_name; }
    const TestFunction& func() const { return m_function; }

private:
    String m_name;
    TestFunction m_function;
    bool m_is_benchmark;
};

void add_test_case_to_suite(const NonnullRefPtr<TestCase>& test_case);

}

#define __TESTCASE_FUNC(x) __test_##x
#define __TESTCASE_TYPE(x) __TestCase_##x

#define TEST_CASE(x)                                                                                    \
    static void __TESTCASE_FUNC(x)();                                                                   \
    struct __TESTCASE_TYPE(x) {                                                                         \
        __TESTCASE_TYPE(x)                                                                              \
        () { add_test_case_to_suite(adopt_ref(*new ::Test::TestCase(#x, __TESTCASE_FUNC(x), false))); } \
    };                                                                                                  \
    static struct __TESTCASE_TYPE(x) __TESTCASE_TYPE(x);                                                \
    static void __TESTCASE_FUNC(x)()

#define __BENCHMARK_FUNC(x) __benchmark_##x
#define __BENCHMARK_TYPE(x) __BenchmarkCase_##x

#define BENCHMARK_CASE(x)                                                                               \
    static void __BENCHMARK_FUNC(x)();                                                                  \
    struct __BENCHMARK_TYPE(x) {                                                                        \
        __BENCHMARK_TYPE(x)                                                                             \
        () { add_test_case_to_suite(adopt_ref(*new ::Test::TestCase(#x, __BENCHMARK_FUNC(x), true))); } \
    };                                                                                                  \
    static struct __BENCHMARK_TYPE(x) __BENCHMARK_TYPE(x);                                              \
    static void __BENCHMARK_FUNC(x)()
