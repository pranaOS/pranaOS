/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#include <listing/list.h>
#include <memory/vmm.h>
#include "queue.h"

struct qelement
{
    void *data;
    struct list_head sibling;
};

/**
 * @brief queue push 
 * 
 * @param q 
 * @param data 
 */
void queue_push(struct queue *q, void *data) {
	struct qelement *qe = kcalloc(1, sizeof(struct qelement));
	qe->data = data;
	list_add_tail(&qe->sibling, q->qhead);
}