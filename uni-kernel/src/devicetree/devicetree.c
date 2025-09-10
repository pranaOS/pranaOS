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

typedef struct dt_header
{
    u32 magic;
    u32 totalsize;
    u32 off_dt_struct;
    u32 off_dt_strings;
    u32 off_mem_rsvmap;
    u32 version;
    u32 last_comp_version;
    u32 boot_cpuid_phys;
    u32 size_dt_strings;
    u32 size_dt_struct;
}* dt_header;

typedef struct dt_reserve_entry
{
    u64 address;
    u64 size;
}* dt_reserve_entry;

struct dt_node
{
    u32 token;
    char name[0];
};

struct dt_prop
{
    u32 data_length;
    u32 name;
    u8 data[0];
};

closure_type(dt_node_begin_handler, boolean, dt_node n, sstring name, int level, dt_node parent);
closure_type(dt_node_end_handler, boolean, dt_node n, int level);
closure_type(dt_prop_handler, boolean, void* dtb, dt_node n, sstring name, dt_prop p);

#define nelem(x) (sizeof((x)) / sizeof((x)[0]))