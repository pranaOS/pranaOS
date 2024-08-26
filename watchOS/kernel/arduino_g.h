/**
 * @file arduino_g.h
 * @author Krisna Pranav
 * @brief Arduino_G
 * @version 6.0
 * @date 2023-06-27
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "types.h"

class ArduinoG
{
	public:
		/**
		 * @param w 
		 * @param h 
		 */
		ArduinoG(int16_t w, int16_t h) : WIDTH(w), HEIGHT(h) {};

		/**
		 * @param speed 
		 */
		virtual void begin(int32_t speed = 0) = 0;

		/**
		 * @param x 
		 * @param y 
		 * @param bitmap 
		 * @param w 
		 * @param h 
		 * @param color 
		 * @param bg 
		 */
	    virtual void drawBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) = 0;

	protected:
		int16_t
		WIDTH,
		HEIGHT;
}; // class ArduinoG
