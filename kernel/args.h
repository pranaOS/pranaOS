#pragma once

/**
 * @brief args present
 * 
 * @param karg 
 * @return int 
 */
int args_present(const char * karg);

/**
 * @brief args value
 * 
 * @param karg 
 * @return char* 
 */
char * args_value(const char * karg);

/**
 * @brief args parse
 * 
 * @param arg 
 */
void args_parse(const char * arg);
