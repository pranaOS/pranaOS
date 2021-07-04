/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <stdio.h>
#include "archs/Arch.h"
#include "system/Streams.h"
#include "system/graphics/EarlyConsole.h"
#include "system/graphics/Font.h"
#include "system/interrupts/Interupts.h"
#include "system/scheduling/Scheduler.h"
#include "system/system/System.h"
#include "system/tasking/Task.h"


static const char *YO_USER =
    "Yo User, I heard you like errors, \n\t"
    "// so I put an error in your error handler\n\t"
    "// so you can get error while you get error";

static const char *const witty_comments[] = {
    "Error Occured :(",
    "Surprise! Haha. Well, this is awkward.",
    "Oh - I know what I did wrong!",
    "Uh... Did I do that?",
    "Oops.",
    "DON'T PANIC!",
    "I'm tired of this ;_;",
    "PC LOAD LETTER",
    "Bad command or file name",
    "OoooOOoOoOF!",
    "My bad.",
    "pranaOS crashed!",
    "Quite honestly, I wouldn't worry myself about that.",
    "This doesn't make any sense!",
    "It's not a good surprise...",
    "Don't do that.",
};

static bool has_panic = false;
static bool nested_panic = false;
