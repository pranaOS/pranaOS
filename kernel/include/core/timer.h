#pragma once 

#include <sys/types.h>

unsigned long timer_get_frequency();

void init_timer();

unsigned long timer_get_cur_ticks();

unsigned long timer_get_period();

unsigned long timer_secs_since_boot();
void          time_since_boot(time_t *sec, time_t *microsec);
