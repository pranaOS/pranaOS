/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkern/types.h>

#define SIGNALS_CNT 32

/* TODO: Add more */
enum SIGNAL_ACTION {
    SIGNAL_ACTION_TERMINATE,
    SIGNAL_ACTION_STOP,
    SIGNAL_ACTION_CONTINUE,
    SIGINT,

};