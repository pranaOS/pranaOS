/**
 * @file devicetree.c
 * @author Krisna Pranav
 * @brief device tree
 * @version 6.0
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <devicetree.h>
#include <runtime.h>

#define DTB_MAGIC 0xd00dfeed

#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE 0x00000002
#define FDT_PROP 0x00000003
#define FDT_NOP 0x00000004
#define FDT_END 0x00000009