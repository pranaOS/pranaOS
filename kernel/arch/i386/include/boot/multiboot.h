#pragma once

struct multiboot
{
	uint32_t flags;
	uint32_t mem_lower;
	uint32_t mem_upper;
	uint32_t boot_device;
	uint32_t cmdline;
	uint32_t mods_count;
	uint32_t mods_addr;
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;

	uint32_t mmap_length;
	uint32_t mmap_addr;

	uint32_t drives_length;
	uint32_t drives_addr;
	uint32_t config_table;
	uint32_t boot_loader_name;
	uint32_t apm_table;

	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
	uint32_t vbe_interface_seg;
	uint32_t vbe_interface_off;
	uint32_t vbe_interface_len;

	uint64_t fb_addr;
	uint32_t fb_pitch;
	uint32_t fb_width;
	uint32_t fb_height;
	uint8_t  fb_bpp;
	uint8_t  fb_type;
	uint8_t  fb_red_field_position;
	uint8_t  fb_red_mask_size;
	uint8_t  fb_green_field_position;
	uint8_t  fb_green_mask_size;
	uint8_t  fb_blue_field_position;
	uint8_t  fb_blue_mask_size;

} __attribute__((packed));

struct vbe_info
{
	uint16_t attributes;
	uint8_t  win_a;
	uint8_t  win_b;
	uint16_t granularity;
	uint16_t winsize;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t realFctPtr;
	uint16_t pitch;

	uint16_t x_res, y_res;
	uint8_t  w_char, y_char, planes, bpp, banks;
	uint8_t  memory_model, bank_size, image_pages;
	uint8_t  reserved0;

	uint8_t red_mask, red_position;
	uint8_t green_mask, green_position;
	uint8_t blue_mask, blue_position;
	uint8_t rsv_mask, rsv_position;
	uint8_t directcolor_attributes;

	uint32_t physbase;
	uint32_t reserved1;
	uint16_t reserved2;

} __attribute__((packed));