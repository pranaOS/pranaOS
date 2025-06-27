/**
 * @file crc.h
 * @author Krisna Pranav
 * @brief crc
 * @version 6.0
 * @date 2025-06-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#ifndef _CRC_H_
#define _CRC_H_

/**
 * @param data 
 * @param len 
 * @return u32 
 */
u32 crc32c(const u8* data, bytes len);

#endif 