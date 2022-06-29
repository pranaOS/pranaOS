/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief circular buffer 
 * 
 */
struct circular_buf_t;

/**
 * @brief circular buf init 
 * 
 * @param buffer 
 * @param size 
 * @return struct circular_buf_t* 
 */
struct circular_buf_t *circular_buf_init(char *buffer, size_t size);

/**
 * @brief circular buf free
 * 
 * @param cbuf 
 */
void circular_buf_free(struct circular_buf_t *cbuf);

/**
 * @brief circular buf reset 
 * 
 * @param cbuf 
 */
void circular_buf_reset(struct circular_buf_t *cbuf);

/**
 * @brief circular buf put
 * 
 * @param cbuf 
 * @param data 
 */
void circular_buf_put(struct circular_buf_t *cbuf, char data);

/**
 * @brief circular buf put2 
 * 
 * @param cbuf 
 * @param data 
 * @return int 
 */
int circular_buf_put2(struct circular_buf_t *cbuf, char data);

/**
 * @brief circular buf get
 * 
 * @param cbuf 
 * @param data 
 * @return int 
 */
int circular_buf_get(struct circular_buf_t *cbuf, char *data);

/**
 * @brief circular buf empty 
 * 
 * @param cbuf 
 * @return true 
 * @return false 
 */
bool circular_buf_empty(struct circular_buf_t *cbuf);

/**
 * @brief circular buf full
 * 
 * @param cbuf 
 * @return true 
 * @return false 
 */
bool circular_buf_full(struct circular_buf_t *cbuf);

/**
 * @brief circular buf capacity 
 * 
 * @param cbuf 
 * @return size_t 
 */
size_t circular_buf_capacity(struct circular_buf_t *cbuf);

/**
 * @brief circular buf size
 * 
 * @param cbuf 
 * @return size_t 
 */
size_t circular_buf_size(struct circular_buf_t *cbuf);