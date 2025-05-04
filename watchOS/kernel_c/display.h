/**
 * @file display.h
 * @author Krisna Pranav
 * @brief Display
 * @version 6.0
 * @date 2023-06-27
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
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

	/**
	 * @param width 
	 * @param height 
	 * @param renderer 
	 */
	FakeDisplay(int width, int height, SDL_Renderer* renderer);

	~FakeDisplay();

	// DISPLAY: ON(or)OFF
	void displayOn();
	void displayOff();

	/**
	 * @param speed 
	 */
	void begin(int32_t speed = 0) override;

	/**
	 * @param x 
	 * @param y 
	 * @param bitmap 
	 * @param w 
	 * @param h 
	 * @param color 
	 * @param bg 
	 */
    void drawBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) override;

	/**
	 * @param x 
	 * @param y 
	 * @param bitmap 
	 * @param color_index 
	 * @param w 
	 * @param h 
	 */
    void drawIndexedBitmap(int16_t x, int16_t y, uint8_t* bitmap, uint16_t* color_index, int16_t w, int16_t h) override;

	/**
	 * @param x 
	 * @param y 
	 * @param bitmap 
	 * @param w 
	 * @param h 
	 */
    void draw3bitRGBBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w, int16_t h) override;

	/**
	 * @param x 
	 * @param y 
	 * @param bitmap 
	 * @param w 
	 * @param h 
	 */
    void draw16bitRGBBitmap(int16_t x, int16_t y, uint16_t* bitmap, int16_t w, int16_t h) override;

	/**
	 * @param x 
	 * @param y 
	 * @param bitmap 
	 * @param w 
	 * @param h 
	 */
    void draw24bitRGBBitmap(int16_t x, int16_t y, uint8_t* bitmap, int16_t w, int16_t h) override;

    SDL_Texture* getTexture() const
    {
    	return this->mainTexture;
    };

    bool isEnabled() const
    {
    	return this->mIsEnabled;
    };

private:
    SDL_Renderer* mainRenderer;
    SDL_Texture* mainTexture = nullptr;
    bool mIsEnabled = false;
}; // class FakeDisplay


extern std::unique_ptr<FakeDisplay> fakeDisplayInstance;
