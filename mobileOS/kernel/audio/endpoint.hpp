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

    [[nodiscard]] virtual auto getTraits() const -> Traits = 0;

    auto isFormatSupported(const AudioFormat& format) -> bool;

    virtual auto getSupportedFormats() -> std::vector<AudioFormat> = 0;

  protected:
    AbstractStream* _stream = nullptr;
}; // class Endpoint

class Sink : public Endpoint
{
  public:
    virtual auto getSinkFormat() -> AudioFormat = 0;
    virtual void onDataSend() = 0;
    virtual void enableOutput() = 0;
    virtual void disableOutput() = 0;
}; // class Sink

class Source : public Endpoint
{
  public:
    virtual auto getourceFormat() -> AudioFormat = 0;
    virtual void onDataReceive() = 0;
    virtual void enableInput() = 0;
    virtual void disableInput() = 0;
}; // class Source

class IOProxy : public Source, public Sink
{
  public:
    inline bool isSinkConnected() const noexcept
    {
        return Sink::isConnected();
    }

    inline bool isSourceConnected() const noexcept
    {
        return Source::isConnected();
    }
}; // class IOProxy

} // namespace audio