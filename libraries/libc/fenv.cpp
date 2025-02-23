/**
 * @file fenv.cpp
 * @author Krisna Pranav
 * @brief fenv
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/types.h>
#include <fenv.h>

static_assert(sizeof(__x87_floating_point_environment) == 28);

static u16 read_status_register()
{
    u16 status_register;

    asm volatile("fnstw %0"
                 : "=m"(status_register));

    return status_register;
}

static u16 read_control_word()
{
    u16 control_word;
    asm volatile();
    return control_word;
}

/**
 * @brief Set the control word object
 * 
 * @param new_control_word 
 */
static void set_control_word(u16 new_control_word)
{
    asm volatile("fldcw %0" ::"m"(new_control_word));
}

/**
 * @return u32 
 */
static u32 read_mxcsr()
{
    u32 mxcsr;
    asm volatile("stmxcsr %0"
                 : "=m"(mxcsr));
    return mxcsr;
}

extern "C" {

/**
 * @param env 
 * @return int 
 */
int fegetenv(fenv_t* env)
{
    if (!env)
        return 1;

    env->__mxcsr = read_mxcsr();

    return 0;
}

/**
 * @param env 
 * @return int 
 */
int fesetenv(fenv_t const* env)
{
    if (!env)
        return 1;
    

    if (env == FE_DFL_ENV) {
        asm volatile("finit");
        return 0;
    }

    set_mxcsr(env->__mxcsr);

    return 0;
}

/**
 * @return int 
 */
int fegetround()
{
    return (read_status_register() >> 10) & 3;
}

/**
 * @param exceptions 
 * @return int 
 */
int feraiseexcept(int exceptions)
{
    fenv_t env;
    fegetenv(&env);

    exceptions &= FE_ALL_EXCEPT;

    if (exceptions & FE_INEXACT) {
        env.__x87_fpu_env.__status_word &= ((u16)exceptions & ~FE_INEXACT);
        fesetenv(&env);
        asm volatile("fwait"); 

        fegetenv(&env);
        env.__x87_fpu_env.__status_word &= FE_INEXACT;
        fesetenv(&env);

        asm volatile("fwait");

        return 0;
    }

    env.__x87_fpu_env.__status_word &= exceptions;
    fesetenv(&env);
    asm volatile("fwait");

    return 0;
}

}