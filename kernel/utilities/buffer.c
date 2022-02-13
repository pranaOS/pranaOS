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
 * @brief circular buffer t
 * 
 */
struct circular_buf_t {
    char *buffer;
    bool full;
    size_t head;
    size_t tail;
    size_t max;
};

/**
 * @brief advance pointer
 * 
 * @param cbuf 
 */
static void advance_pointer(struct circular_buf_t *cbuf) {
    if (cbuf->full) {
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

size_t circular_buf_capacity(struct circular_buf_t *cbuf) {
    return cbuf->max;
}

size_t circular_buf_size(struct circular_buf_t *cbuf) {
    size_t size = cbuf->max;

    if (cbuf->full) {
        // ..
    }

    return size;
}

void circular_buf_put(struct circular_buf_t *cbuf, char data) {
    cbuf->buffer[cbuf->head] = data;

    advance_pointer(cbuf);
}