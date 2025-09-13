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
static struct prop_value_map
{
    sstring name;
    int type;
} prop_value_map[] = {
    {ss_static_init("compatible"), DT_VALUE_STRINGLIST},
    {ss_static_init("model"), DT_VALUE_STRING},
    {ss_static_init("phandle"), DT_VALUE_U32},
    {ss_static_init("status"), DT_VALUE_STRING},
    {ss_static_init("#address-cells"), DT_VALUE_U32},
    {ss_static_init("#size-cells"), DT_VALUE_U32},
    {ss_static_init("#interrupt-cells"), DT_VALUE_U32},
    {ss_static_init("interrupt-parent"), DT_VALUE_PHANDLE},
    {ss_static_init("reg"), DT_VALUE_REG},
    {ss_static_init("virtual-reg"), DT_VALUE_U32},
    {ss_static_init("device_type"), DT_VALUE_STRING},
    {ss_static_init("bootargs"), DT_VALUE_STRING},
    {ss_static_init("stdout-path"), DT_VALUE_STRING},
    {ss_static_init("stdin-path"), DT_VALUE_STRING},
    {ss_static_init("clock-frequency"), DT_VALUE_FREQ},
    {ss_static_init("timebase-frequency"), DT_VALUE_FREQ},
    {ss_static_init("mmu-type"), DT_VALUE_STRING},
    {ss_static_init("cpu"), DT_VALUE_PHANDLE},
};

unsigned int dt_prop_cell_count(dt_prop prop)
{
    return dt_u32(prop->data_length) / sizeof(u32);
}

u32 dt_prop_get_cell(dt_prop prop, unsigned int index)
{
    void* ptr = prop->data + index * sizeof(u32);
    return dt_u32(*((u32*)ptr));
}

sstring dtb_string(void* dtb, u64 off)
{
    dt_header fdt = dtb;
    u64 end = dt_u32(fdt->size_dt_strings);
    if(off >= end)
        return ss("(bad string offset)");
    return sstring_from_cstring((char*)fdt + dt_u32(fdt->off_dt_strings) + off, end - off);
}

#define MAX_NODE_DEPTH 16

/**
 * @param dtb
 * @param dn
 * @param nbh
 * @param neh
 * @param ph
 */
void dtb_walk_internal(void* dtb, dt_node dn, dt_node_begin_handler nbh, dt_node_end_handler neh, dt_prop_handler ph)
{
    dt_node nstack[MAX_NODE_DEPTH];
    sstring name;
    int nodelevel = 0;
    dt_header fdt = dtb;
    u8* n = (u8*)dn;
    if(dt_u32(fdt->magic) != DTB_MAGIC)
    {
        console("dtb_walk_internal: bad magic\n");
        return;
    }
    u8* end = dtb + dt_u32(fdt->totalsize);
    while(true)
    {
        if(n < (u8*)dtb || n >= end)
        {
            console("dtb_walk_internal: parsing outside blob; aborted\n");
            return;
        }
        u32 token = dt_u32(*(u32*)n);
        void* curtok = n;
        n += 4;
        void* data = n;
        switch(token)
        {
        case FDT_BEGIN_NODE:
            name = sstring_from_cstring(data, end - n);
            if(nbh && !apply(nbh, (dt_node)curtok, name, nodelevel, nodelevel ? nstack[nodelevel - 1] : INVALID_ADDRESS))
                return;
            n += pad(name.len + 1, 4);
            nstack[nodelevel++] = curtok;
            break;
        case FDT_END_NODE:
            nodelevel--;
            if(neh && !apply(neh, nstack[nodelevel], nodelevel))
                return;
            if(nodelevel == 0)
                return;
            break;
        case FDT_PROP:
        {
            dt_prop p = data;
            name = dt_string(dtb, p->name);
            if(ph && !apply(ph, dtb, nstack[nodelevel - 1], name, p))
                return;
            n += sizeof(*p) + pad(dt_u32(p->data_length), 4);
            break;
        }
        case FDT_NOP:
            break;
        case FDT_END:
            console("dtb_walk_internal: unexpected FDT_END\n");
            return;
        default:
            console("dtb_walk_internal: unknown token\n");
            return;
        }
    }
}