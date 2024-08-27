/**
 * @file display.h
 * @author Krisna Pranav
 * @brief Display
 * @version 6.0
 * @date 2023-06-27
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include "arduino_gfx.h"
#include "defines.h"

class FakeDisplay: public Arduino_G
{
public:
	const int width;
	const int height;

	FakeDisplay(int width, int height, SDL_Renderer* renderer);
	~FakeDisplay();

	void displayOn();
	void displayOff();
}; // class FakeDisplay
