/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "list.h"
#include <stddef.h>
#include <stdint.h>

/**
 * @brief plist_head[node list]
 * 
 */
struct plist_head {
    struct list_head node_list;
};

struct plist_node {
    int prio;
    struct list_head prio_list;
    struct list_head node_list;
};

#define PLIST_HEAD_INIT(head)                         \
	{                                                 \
		.node_list = LIST_HEAD_INIT((head).node_list) \
	}

#define PLIST_HEAD(head) \
	struct plist_head head = PLIST_HEAD_INIT(head)

#define PLIST_NODE_INIT(node, __prio)                  \
	{                                                  \
		.prio = (__prio),                              \
		.prio_list = LIST_HEAD_INIT((node).prio_list), \
		.node_list = LIST_HEAD_INIT((node).node_list), \
	}

static inline void
plist_node_init(struct plist_node *node, int prio) {
    node->prio = prio;
    INIT_LIST_HEAD(&node->prio_list);
    INIT_LIST_HEAD(&node->node_list);
}

extern void plist_ad(struct plist_node *node, struct plist_head *head);
extern void plist_delete(struct plist_node *node, struct plist_head *head);
extern void plist_requeue(struct plist_node *node, struct plist_head *head);

#define plist_for_each(pos, head) \
	list_for_each_entry(pos, &(head)->node_list, node_list)

#define plist_for_each_continue(pos, head) \
	list_for_each_entry_continue(pos, &(head)->node_list, node_list)

#define plist_for_each_safe(pos, n, head) \
	list_for_each_entry_safe(pos, n, &(head)->node_list, node_list)

#define plist_for_each_entry(pos, head, mem) \
	list_for_each_entry(pos, &(head)->node_list, mem.node_list)

#define plist_for_each_entry_continue(pos, head, m) \
	list_for_each_entry_continue(pos, &(head)->node_list, m.node_list)

#define plist_for_each_entry_safe(pos, n, head, m) \
	list_for_each_entry_safe(pos, n, &(head)->node_list, m.node_list)

static inline void 
plist_head_init(struct plist_head *head) {
    INIT_LIST_HEAD(&head->node_list);
}

static inline int
plist_head_empty(const struct plist_head *head) {
    return list_empty(&head->node_list);
}

static inline int
plist_node_empty(const struct plist_node *node) {
    return list_empty(&node->node_list);
}

#define plist_first_entry(head, type, member) \
	container_of(plist_first(head), type, member)

#define plist_last_entry(head, type, member) \
	container_of(plist_last(head), type, member)

#define plist_next(pos) \
	list_next_entry(pos, node_list)

#define plist_prev(pos) \
	list_prev_entry(pos, node_list)