#pragma once

#include <stdint.h>

/**
 * @brief inet ntop
 * 
 * @param src 
 * @param dst 
 * @param len 
 * @return char* 
 */
char *inet_ntop(uint32_t src, char *dst, uint16_t len);