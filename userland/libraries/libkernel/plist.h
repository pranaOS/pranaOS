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
