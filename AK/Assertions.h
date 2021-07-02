/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#pragma once

#if defined(KERNEL)
#    include <Kernel/Assertions.h>
#else
#    include <assert.h>
#    ifndef __pranaos__
#        define VERIFY assert
#        define VERIFY_NOT_REACHED() assert(false)
#        define TODO VERIFY_NOT_REACHED
#    endif
#endif
