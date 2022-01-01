#pragma once

#include <sys/types.h>

unsigned long timer_get_frequency();

void init_timer();

unsigned long timer_get_cur_ticks();