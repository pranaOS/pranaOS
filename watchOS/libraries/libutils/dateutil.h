/**
 * @file dateutil.h
 * @author Krisna Pranav
 * @brief DateUtil
 * @version 6.0
 * @date 2023-07-04
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once


class DateUtil {
public:

	// weekdays
	static uint8_t weekday(uint year, uint8_t month, uint8_t day);

	// get days in mnth
	static uin8_t getDaysInMonth(uint year, int month);

	// week days
	static void weekdayName(char *dayInWeekStr, uint year, uint8_t month, uint8_t day);

	// month name short
	static void monthNameShort(char (&name)[4], int month);
};
