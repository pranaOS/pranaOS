/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/String.h>
#include <base/Vector.h>

namespace Test {

enum class Result {
    Pass,
    Fail,
    Skip,
    Crashed,
};

struct Case {
    String name;
    Result result;
    String details;
};

struct Suite {
    String name;
    Result most_severe_test_result { Result::Pass };
    Vector<Case> tests {};
};

struct Counts {
    unsigned tests_failed { 0 };
    unsigned tests_passed { 0 };
    unsigned tests_skipped { 0 };
    unsigned suites_failed { 0 };
    unsigned suites_passed { 0 };
    unsigned files_total { 0 };
};

}
