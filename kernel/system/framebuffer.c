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