/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_ALGO_DYNAMIC_ARRAY_H
#define _KERNEL_ALGO_DYNAMIC_ARRAY_H

// includes
#include <libkernel/types.h>

struct dynamic_array
{
  void* data;
  uint32_t capacity; 
  uint32_t size;
  uint32_t element_size; 
};

typedef struct dynamic_array dynamic_array_t;

int dynamic_array_init(dynamic_array_t* v, uint32_t element_size);
int dynamic_array_init_of_size(dynamic_array_t* v, uint32_t element_size, uint32_t capacity);
int dynamic_array_free(dynamic_array_t* v);

void* dynamic_array_get(dynamic_array_t* v, int index);
int dynamic_array_push(dynamic_array_t* v, void* element);
int dynamic_array_pop(dynamic_array_t* v);
int dynamic_array_clear(dynamic_array_t* v);

#endif 