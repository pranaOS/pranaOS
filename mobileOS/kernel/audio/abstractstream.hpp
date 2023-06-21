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
            StreamHalfUsed,
            StreamEmpty,
            StreamOverflow,
            StreamUnderFlow
        };

        class EventListener {
          public:
            virtual void onEvent(AbstractStream *stream, Event event) = 0;
        };

        struct Span {
            std::uint8_t *data = nullptr;
            std::size_t dataSize = 0;

            constexpr std::uint8_t *dataEnd() const noexcept {
                return data + dataSize;
            }
        };
    };
}