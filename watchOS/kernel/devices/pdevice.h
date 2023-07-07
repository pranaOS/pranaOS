/**
 * @file oswdevice.h
 * @author Krisna Pranav
 * @brief oswdevice
 * @version 1.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <set>
#include <Arduino.h>

class OswDevice {
public:
  virtual void setup() = 0;
  virtual void update() = 0;
  virtual void reset() = 0;
  virtual void stop() = 0;

  virtual const char* getName() = 0;

  static const std::set<OswDevice*>* getAllDevice() {
      return &allDevice;
  }

private:
  static std::set<OswDevice*> allDevice;

protected:
  OswDevice();
  virtual ~OswDevice();
};