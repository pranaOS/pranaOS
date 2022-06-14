#pragma once

/**
 * @brief assert failed
 * 
 * @param file 
 * @param line 
 * @param func 
 * @param cond 
 */
extern void __assert_failed(const char * file, int line, const char * func, const char * cond);

/**
 * @brief assert
 * 
 * @param condition
 */
#define assert(condition) do { if (!(condition)) __assert_failed(__FILE__,__LINE__,__func__,#condition); } while (0)