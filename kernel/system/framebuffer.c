#include <filesystem/vfs.h>
#include <memory/pmm.h>
#include <memory/vmm.h>
#include <proc/task.h>
#include <libkernel/math.h>
#include <libkernel/string.h>
#include "psf.h"
#include "framebuffer.h"

#define VIDEO_VADDR 0xFC000000
#define TEXT_COLOR 0xFFFFFF
#define BACKGROUND_COLOR 0x000000

static struct framebuffer *current_buffer;

void framebuffer_init(struct multiboot_tag_framebuffer *multiboot_framebuffer) {
	current_buffer = kcalloc(1, sizeof(struct framebuffer));
	current_buffer->addr = multiboot_framebuffer->common.framebuffer_addr;
	current_buffer->bpp = multiboot_framebuffer->common.framebuffer_bpp;
	current_buffer->pitch = multiboot_framebuffer->common.framebuffer_pitch;
	current_buffer->width = multiboot_framebuffer->common.framebuffer_width;
	current_buffer->height = multiboot_framebuffer->common.framebuffer_height;

	uint32_t screen_size = current_buffer->height * current_buffer->pitch;
	uint32_t blocks = div_ceil(screen_size, PMM_FRAME_SIZE);
	for (uint32_t i = 0; i < blocks; ++i)
		vmm_map_address(
			vmm_get_directory(),
			VIDEO_VADDR + i * PMM_FRAME_SIZE,
			current_buffer->addr + i * PMM_FRAME_SIZE,
			I86_PTE_PRESENT | I86_PTE_WRITABLE);
}

struct framebuffer *get_framebuffer() {
	return current_buffer;
}