#pragma once

struct time_s {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_day;
	int tm_mon;
	int tm_year;
	int tm_wdag;
	int tm_yday;
	int tm_isdst;
};

void init_time();

struct time_s *get_time();

void time_add_delta(unsigned int milliseconds);