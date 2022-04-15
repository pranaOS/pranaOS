#pragma once

#include <memory/vmm.h>
#include "time.h"

extern volatile uint64_t jiffies;
volatile uint64_t boot_seconds, current_seconds;

struct time epoch_time = {
	.year = 1970,
	.month = 1,
	.day = 1,
	.hour = 0,
	.minute = 0,
	.second = 0,
};

struct time current_time;