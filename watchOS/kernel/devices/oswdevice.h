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
  

private:
  static std::set<OswDevice*> allDevice;

protected:
  OswDevice();
  virtual ~OswDevice();
};