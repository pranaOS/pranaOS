/**
 * @file codec.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-06-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "audioformat.hpp"
#include <cstdint>

namespace audio {
    class AbstractStream {
      public:
        enum class Event {
            NoEvent,
            StreamFull,
        };
    };
}