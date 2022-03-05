#pragma once

#include <stdint.h>

/**
 * @brief pic functions
 * 
 */
void pic_remap();
void pic_set_mask(unsigned char irq_line);
void pic_clear_mask(unsigned char irq_line);

/**
 * @brief pic initialize function
 * 
 */
void pic_init();