/**
 * @file assertions.h
 * @author Krisna Pranav
 * @brief Assertions
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

 #pragma once

 #if defined(KERNEL)
 #    include <kernel/assertions.h>
 #else
 #    include <assert.h>
 #    define VERIFY assert
 #    define VERIFY_NOT_REACHED() assert(false)
 static constexpr bool TODO = false;
 #    define TODO() VERIFY(TODO)
 #endif
