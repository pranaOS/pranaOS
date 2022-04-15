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

void set_boot_seconds(uint64_t bs) {
	boot_seconds = bs;
}

void set_current_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
	current_time.year = year;
	current_time.month = month;
	current_time.day = day;
	current_time.hour = hour;
	current_time.minute = minute;
	current_time.second = second;
	current_seconds = get_seconds(NULL);
}