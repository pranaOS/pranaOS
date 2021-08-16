/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libtest/TestCase.h>
#include <base/Format.h>
#include <libtest/TestSuite.h>

#ifdef KERNEL
#    define TEST_MAIN test_main
#else
#    define TEST_MAIN main
#endif

int TEST_MAIN(int argc, char** argv)
{
    if (argc < 1 || !argv[0] || '\0' == *argv[0]) {
        warnln("Test main does not have a valid test name!");
        return 1;
    }
    int ret = ::Test::TestSuite::the().main(argv[0], argc, argv);
    ::Test::TestSuite::release();
    return ret;
}
