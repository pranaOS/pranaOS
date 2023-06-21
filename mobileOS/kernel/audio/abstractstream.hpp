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

            constexpr void reset() noexcept {
                data = nullptr;
                dataSize = 0;
            }

            inline bool operator==(const Span &other) const noexcept {
                return data == other.data && dataSize == other.dataSize;
            }

            inline bool operator!=(const Span &other) const noexcept {
                return !operator==(other);
            }
        };

        struct Traits {
            std::size_t blockSize = 0;
            AudioFormat format = nullFormat;
        };

        virtual ~AbstractStream() = default;

        virtual void registerListener(EventListener *listener) = 0;

        virtual void unregisterListeners(EventListener *listener) = 0;

        virtual bool push(void *data, std::size_t dataSize) = 0;

        virtual bool push(const Span &span) = 0;

        virtual bool push() = 0;

        virtual bool pop(Span &span) = 0;

        virtual bool reserve(Span &span) = 0;

        virtual void commit() = 0;

        virtual void release() = 0;

        virtual bool peek(Span &span) = 0;

        virtual void consume() = 0;

        virtual void unpeek() = 0;

        virtual void reset() = 0;

        [[nodiscard]] virtual auto getInputTraits() const noexcept -> Traits = 0;

        [[nodiscard]] virtual bool isEmpty() const noexcept = 0;

        [[nodiscard]] virtual bool isFull() const noexcept = 0;
    };
}