/**
 * @file endpoint.hpp
 * @author Krisna Pranav
 * @brief Endpoint for audio format and abstract streamer
 * @version 0.1
 * @date 2024-07-30
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "abstractstream.hpp"
#include "audioformat.hpp"
#include <chrono>
#include <cstdint>
#include <optional>
#include <vector>

namespace audio
{
class Endpoint
{
  public:
    struct Traits
    {
        bool usesDMA = false;
        std::optional<std::size_t> blockSizeConstraint = std::nullopt;
        std::optional<std::chrono::milliseconds> timeConstraint = std::nullopt;
    }; // struct Traits

    Endpoint() = default;

    /**
     * @param stream
     */
    void connectStream(AbstractStream& stream);

    void disconnectStream();
    bool isConnected() const noexcept;
}; // class Endpoint
} // namespace audio