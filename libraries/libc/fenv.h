/**
 * @file fenv.h
 * @author Krisna Pranav
 * @brief fenv
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

typedef struct fenv_t {
    uint32_t __mxcsr;
} fenv_t;

typedef uint16_t fexcept_t;

#define FE_TONEAREST 0
#define FE_DOWNWARD 1
#define FE_UPWARD 2
#define FE_TOWARDZERO 3

int fesetround(int round);
int fegetround(void);

__END_DECLS