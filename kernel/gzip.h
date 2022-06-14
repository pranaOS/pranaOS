#pragma once

#include <stdint.h>

/**
 * @brief gzip decompress
 * 
 * @return int 
 */
extern int gzip_decompress(void);

/**
 * @brief gzip input ptr
 * 
 */
extern uint8_t * gzip_inputPtr;

/**
 * @brief gzip output ptr
 * 
 */
extern uint8_t * gzip_outputPtr;