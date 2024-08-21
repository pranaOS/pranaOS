/**
 * @file assertions.h
 * @author Krisna Pranav
 * @brief Assertions
 * @version 6.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#ifndef MOD_TEST_SUITE

#    if defined(KERNEL)
#        include <kernel/assertions.h>
#    else
#        include <assert.h>
#        ifndef __pranaos__
#            define ASSERT assert
#            define ASSERT_NOT_REACHED() assert(false)
#            define RELEASE_ASSERT assert
#            define TODO ASSERT_NOT_REACHED
#        endif
#    endif

#endif
