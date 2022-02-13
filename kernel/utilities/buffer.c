/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stddef.h>
#include <stdint.h>
#include <buffer.h>
#include <memory/vmm.h>

/**
 * @brief circular buf
 * 
 */
struct circular_buf_t {
	char *buffer;
	size_t head;
	size_t tail;
	size_t max;	 
	bool full;
};

/**
 * @brief advance pointer
 * 
 * @param cbuf 
 */
static void advance_pointer(struct circular_buf_t *cbuf)  {
	if (cbuf->full)
	{
		cbuf->tail = (cbuf->tail + 1) % cbuf->max;
	}

	cbuf->head = (cbuf->head + 1) % cbuf->max;

	cbuf->full = (cbuf->head == cbuf->tail);
}

/**
 * @brief retreat pointer
 * 
 * @param cbuf 
 */
static void retreat_pointer(struct circular_buf_t *cbuf) {
	cbuf->full = false;
	cbuf->tail = (cbuf->tail + 1) % cbuf->max;
}

/**
 * @brief circular buf init
 * 
 * @param buffer 
 * @param size 
 * @return struct circular_buf_t* 
 */
struct circular_buf_t *circular_buf_init(char *buffer, size_t size) {
	struct circular_buf_t *cbuf = kcalloc(1, sizeof(struct circular_buf_t));

	cbuf->buffer = buffer;
	cbuf->max = size;
	circular_buf_reset(cbuf);

	return cbuf;
}

/**
 * @brief circular buf free
 * 
 * @param cbuf 
 */
void circular_buf_free(struct circular_buf_t *cbuf) {
	kfree(cbuf);
}

/**
 * @brief circular buf reset
 * 
 * @param cbuf 
 */
void circular_buf_reset(struct circular_buf_t *cbuf) {
	cbuf->head = 0;
	cbuf->tail = 0;
	cbuf->full = false;
}

/**
 * @brief circular buf size 
 * 
 * @param cbuf 
 * @return size_t 
 */
size_t circular_buf_size(struct circular_buf_t *cbuf) {
	size_t size = cbuf->max;

	if (!cbuf->full) {
		if (cbuf->head >= cbuf->tail) {
			size = (cbuf->head - cbuf->tail);
		}
		else {
			size = (cbuf->max + cbuf->head - cbuf->tail);
		}
	}

	return size;
}

/**
 * @brief circular buf capacity
 * 
 * @param cbuf 
 * @return size_t 
 */
size_t circular_buf_capacity(struct circular_buf_t *cbuf) {
	return cbuf->max;
}

/**
 * @brief circular buf put
 * 
 * @param cbuf 
 * @param data 
 */
void circular_buf_put(struct circular_buf_t *cbuf, char data) {
	cbuf->buffer[cbuf->head] = data;

	advance_pointer(cbuf);
}

/**
 * @brief circular buf put2
 * 
 * @param cbuf 
 * @param data 
 * @return int 
 */
int circular_buf_put2(struct circular_buf_t *cbuf, char data) {
	int r = -1;

	if (!circular_buf_full(cbuf))
	{
		cbuf->buffer[cbuf->head] = data;
		advance_pointer(cbuf);
		r = 0;
	}

	return r;
}

/**
 * @brief circular buf get
 * 
 * @param cbuf 
 * @param data 
 * @return int 
 */
int circular_buf_get(struct circular_buf_t *cbuf, char *data) {
	int r = -1;

	if (!circular_buf_empty(cbuf))
	{
		*data = cbuf->buffer[cbuf->tail];
		retreat_pointer(cbuf);

		r = 0;
	}

	return r;
}

/**
 * @brief circular buf empty
 * 
 * @param cbuf 
 * @return true 
 * @return false 
 */
bool circular_buf_empty(struct circular_buf_t *cbuf) {
	return (!cbuf->full && (cbuf->head == cbuf->tail));
}

/**
 * @brief circular buf full
 * 
 * @param cbuf 
 * @return true 
 * @return false 
 */
bool circular_buf_full(struct circular_buf_t *cbuf) {
	return cbuf->full;
}