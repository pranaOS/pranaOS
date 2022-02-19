/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>
#include <listing/list.h>

/**
 * @brief queue struct 
 * 
 */
struct queue
{
	struct list_head *qhead;
	uint32_t number_of_items;
};

/**
 * @brief queue push 
 * 
 * @param q 
 * @param data 
 */
void queue_push(struct queue *q, void *data);

/**
 * @brief queue pop
 * 
 * @param q 
 * @return void* 
 */
void *queue_pop(struct queue *q);

/**
 * @brief queue peek
 * 
 * @param q 
 * @return void* 
 */
void *queue_peek(struct queue *q);