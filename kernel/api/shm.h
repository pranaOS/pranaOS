/**
 * @file shm.h
 * @author Krisna Pranav
 * @brief shm
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

#define SHM_READ 0x1u
#define SHM_WRITE 0x2u
#define SHM_SHARE 0x4u

__DECL_BEGIN

struct shm {
	void* ptr;
	size_t size;
	int id;
};

__DECL_END
