/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libkernel/log.h>
#include <libkernel/types.h>
#include <mem/vmm/vmm.h>
#include <platform/aarch32/vmm/pte.h>

void page_desc_init(page_desc_t* pte)
{
    pte->data = 0;
    pte->one = 1;
    pte->ap1 = 0b01; 
    pte->ap2 = 0;
    pte->c = 1;
    pte->s = 1;
    pte->b = 1;
    pte->tex = 0b001;
}

void page_desc_set_attrs(page_desc_t* pte, uint32_t attrs)
{
    if ((attrs & PAGE_DESC_PRESENT) == PAGE_DESC_PRESENT) {
        pte->one = 1;
    }
    if ((attrs & PAGE_DESC_USER) == PAGE_DESC_USER) {
        pte->ap1 = 0b10 | page_desc_is_writable(*pte);
    }
    if ((attrs & PAGE_DESC_WRITABLE) == PAGE_DESC_WRITABLE) {
        pte->ap1 |= 0b01;
    }
    if ((attrs & PAGE_DESC_NOT_CACHEABLE) == PAGE_DESC_NOT_CACHEABLE) {
        pte->c = 0;
    }
}

void page_desc_del_attrs(page_desc_t* pte, uint32_t attrs)
{
    if ((attrs & PAGE_DESC_PRESENT) == PAGE_DESC_PRESENT) {
        pte->one = 0;
    }
    if ((attrs & PAGE_DESC_WRITABLE) == PAGE_DESC_WRITABLE) {
        pte->ap1 = 0b10;
    }
    if ((attrs & PAGE_DESC_USER) == PAGE_DESC_USER) {
        pte->ap1 = 0b01;
    }
    if ((attrs & PAGE_DESC_NOT_CACHEABLE) == PAGE_DESC_NOT_CACHEABLE) {
        pte->c = 1;
    }
}