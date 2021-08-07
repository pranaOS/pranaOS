/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_TASKING_SIGNAL_H
#define _KERNEL_TASKING_SIGNAL_H

// includes
#include <libkernel/types.h>

#define SIGNALS_CNT 32

enum SIGNAL_ACTION {
    SIGNAL_ACTION_TERMINATE,
    SIGNAL_ACTION_STOP,
    SIGNAL_ACTION_CONTINUE,
    SIGINT,

};

#endif