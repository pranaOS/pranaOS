#pragma once

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define AMOUNT_OF_PAGES_PER_TABLE 1024 
#define AMOUNT_OF_PAGE_TABLES_PER_DIR 1024

#define PAGE_BUFFER_LOCATION 0x1100000

typedef struct page {
    uint32_t present;
    uint32_t rw;
    uint32_t user;
    uint32_t accessed;
    uint32_t dirty;
    uint32_t unused;
    uint32_t frame;
} page_t;