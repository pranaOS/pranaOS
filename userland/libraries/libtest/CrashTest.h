/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Function.h>
#include <base/String.h>

namespace Test {

class Crash {
public:
    enum class RunType {
        UsingChildProcess,
        UsingCurrentProcess,
    };

    enum class Failure {
        DidNotCrash,
        UnexpectedError,
    };

    Crash(String test_type, Function<Crash::Failure()> crash_function);

    bool run(RunType run_type = RunType::UsingChildProcess);

private:
    String m_type;
    Function<Crash::Failure()> m_crash_function;
};

}
