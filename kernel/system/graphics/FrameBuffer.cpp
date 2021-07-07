/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libabi/Paths.h>
#include "system/Streams.h"
#include <libmath/MinMax.h>
#include "archs/Arch.h"
#include "system/graphics/Graphics.h"
#include "system/interrupts/Interupts.h"
#include "system/node/Node.h"
#include "system/scheduling/Scheduler.h"

static uintptr_t _framebuffer_physical = 0;
static uintptr_t _framebuffer_virtual = 0;
static int _framebuffer_width = 0;
static int _framebuffer_height = 0;
static int _framebuffer_pitch = 0;
static int _framebuffer_bpp = 0;

struct Framebuffer : public FsNode
{
private:

public:
    Framebuffer() : FsNode(J_FILE_TYPE_DEVICE)
    {
    }

    ~Framebuffer()
    {
    }

}