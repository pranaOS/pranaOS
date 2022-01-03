#pragma once

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define AMOUNT_OF_PAGES_PER_TABLE     1024
#define AMOUNT_OF_PAGE_TABLES_PER_DIR 1024

#define PAGE_BUFFER_LOCATION 0x1100000

typedef struct page
{
	uint32_t present  : 1;  
	uint32_t rw       : 1;  
	uint32_t user     : 1;  
	uint32_t accessed : 1;  
	uint32_t dirty    : 1;  
	uint32_t unused   : 7;  
	uint32_t frame    : 20; 
} page_t;

typedef struct page_table
{
	page_t pages[AMOUNT_OF_PAGES_PER_TABLE];
} page_table_t;

typedef struct page_directory
{
	page_table_t *tables[AMOUNT_OF_PAGE_TABLES_PER_DIR];
	uint32_t      tablesPhysical[AMOUNT_OF_PAGE_TABLES_PER_DIR];
	uint32_t      physicalAddress;
} page_directory_t;

int get_frame(unsigned int frame_addr);

page_directory_t *get_kernel_dir();
page_directory_t *get_current_dir();
void              set_current_dir(page_directory_t *new);

unsigned long phys_freeram_amount();
unsigned long phys_freeswap_amount();
unsigned long sharedram_amount();
unsigned long bufferram_amount();

int alloc_frame(page_t *page, int is_kernel, int is_writable_from_userspace);
int realloc_frame(page_t *page, int is_kernel, int is_writable_from_userspace);
int flagforce_alloc_frame(page_t *page, int is_kernel, int writeable);

int map_frame(page_t *page, unsigned int addr, int remap, int is_kernel,
              int is_writable_from_userspace);

phys_addr_t virt_to_phys(void *virtual_address);

void switch_page_directory(page_directory_t *dir);

page_t *get_page(uint32_t virtual_address, int make_page_table,
                 page_directory_t *dir);

void alloc_virt(offset_t start, size_t size, int user);

page_directory_t *duplicate_current_page_directory();

int init_paging();

int map_physical_to_virtual(phys_addr_t *physical_address,
                            void *virtual_address, size_t size, int is_kernel,
                            int               is_writable_from_userspace,
                            page_directory_t *dir);

void copy_stack_to_new_addressspace(page_directory_t *newdir);

int map_mem(uint32_t startaddr, uint32_t endaddr, int is_kernel,
            int is_writable_from_userspace);
int remap_mem(uint32_t startaddr, uint32_t endaddr, int is_kernel,
              int is_writable_from_userspace);

void clear_page_directory(page_directory_t *dir);

int identity_map_memory_block(uint32_t startaddr, uint32_t endaddr,
                              int is_kernel, int is_writable_from_userspace,
                              page_directory_t *dir);

void debug_paging_print(page_directory_t *dir);