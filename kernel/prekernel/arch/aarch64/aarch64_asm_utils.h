/**
 * @file aarch64_asm_utils.h
 * @author Krisna Pranav
 * @brief aarch64 asm utils
 * @version 0.1
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

/**
 * @return n
 * 
 */
extern "C" void wait_cycles(int n);

/**
 * @return vector_table
 * 
 */
extern "C" void el1_vector_table_install(void* vector_table);

extern "C" [[noreturn]] void return_from_el2();
extern "C" [[noreturn]] void return_from_el3();
