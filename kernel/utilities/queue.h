/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <stdint.h>
#include <listing/list.h>

struct queue
{
    struct list_head *qhead;
    uint32_t number_of_items;
};

void queue_push(struct queue *q, void *data);