/**
 * @file Config.h
 * @author Krisna Pranav
 * @brief Config
 * @version 0.1
 * @date 2023-06-16
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, pranaOS Developers
 * 
 */

#pragma once

#include <Arduino.h>

#define WATCH_2020_V3

#define FULL_NAME "pranaWatch 1.0"
#define APP_NAME "pranaWatch"
#define OS_VERSION "1.0"

#define BATTERY_LOW 30
#define BATTERY_VERY_LOW 20

#define CPU_FREQUENCY_LOW 20
#define CPU_FREQUENCY_HIGH 240
#define CROSS_SWIPE_TOLERANCE 12

#define COLOR_BACKGROUND 0x0000

#define COLOR_MAIN_1 0x07E0
#define COLOR_MAIN_2 0x001F
#define COLOR_MAIN_3 0x03EF

#define COLOR_WARN 0xF800

#define DISPLAY_ADJUST 80

#define SERIAL_RATE 19200

#define SLEEPCYCLE_MS 50
#define TICK_WAKEUP 10

#define TICKER_BATTERY 1200
#define TICKER_DATETIME 2
#define TICKER_CLOCKS 80
#define TICKER_STEPS 20
#define TICKER_DETECT_ONHAND 5

#define DETECT_ONHAND_ACC_THRESHOLD 50

#define INACTIVITY_ONHAMD_TRESHOLD 5
#define INACTIVITY_TRESHOLD 5

#define LONGTOUCH_UNLOCK 400
#define LONGTOUCH_INAPP 400
#define TOUCH_LIFETIME 2500