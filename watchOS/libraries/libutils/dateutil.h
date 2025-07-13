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

	/**
	 * @param year 
	 * @param month 
	 * @param day 
	 * @return uint8_t 
	 */
	static uint8_t weekday(uint year, uint8_t month, uint8_t day);

	/**
	 * @param year 
	 * @param month 
	 * @return uin8_t 
	 */
	static uin8_t getDaysInMonth(uint year, int month);

	/**
	 * @param dayInWeekStr 
	 * @param year 
	 * @param month 
	 * @param day 
	 */
	static void weekdayName(char *dayInWeekStr, uint year, uint8_t month, uint8_t day);

	/**
	 * @param name 
	 * @param month 
	 */
	static void monthNameShort(char (&name)[4], int month);

	/**
	 * @param name 
	 * @param year 
	 * @return * void 
	 */
    static void yearNameShort(char (&name)[4], int year);
};
