/**
 * @file audioformat.hpp
 * @author Krisna Pranav
 * @brief Audio Format
 * @version 1.0
 * @date 2023-06-20
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <chrono>
#include <set>
#include <string>
#include <vector>

namespace audio
{
class AudioFormat
{
  private:
    unsigned int sampleRate = 0;
    unsigned int bitWidth = 0;
    unsigned int channels = 0;

  public:
    constexpr AudioFormat() = default;
    constexpr AudioFormat(unsigned int sampleRate, unsigned int bitWidth, unsigned int channels) noexcept
        : sampleRate(sampleRate), bitWidth(bitWidth), channels(channels)
    {
    }

    auto getSampleRates() const noexcept -> unsigned int;
    auto getBitWidth() const noexcept -> unsigned int;
    auto getChannels() const noexcept -> unsigned int;

    auto getBitrate() const noexcept -> unsigned long int;
    auto toString() const -> std::string;

    /**
     * @brief bytesToMicroseconds
     *
     * @param bytes
     * @return std::chrono::microseconds
     */
    auto bytesToMicroseconds(unsigned int bytes) const noexcept -> std::chrono::microseconds;

    /**
     * @brief microsecondsToBytes
     *
     * @param duration
     * @return unsigned int
     */
    auto microsecondsToBytes(std::chrono::microseconds duration) const noexcept -> unsigned int;

    auto operator==(const AudioFormat& other) const -> bool;
    auto operator!=(const AudioFormat& other) const -> bool;
    auto operator>(const AudioFormat& other) const -> bool;
    auto operator<(const AudioFormat& other) const -> bool;
    auto operator<=(const AudioFormat& other) const -> bool;
    auto operator>=(const AudioFormat& other) const -> bool;

    auto isValid() const noexcept -> bool;
    auto isNull() const noexcept -> bool;

    /**
     * @brief makeMatrics
     *
     * @param sampleRate
     * @param bitWidths
     * @param channels
     * @return std::vector<AudioFormat>
     */
    static auto makeMatrics(std::set<unsigned int> sampleRate, std::set<unsigned int> bitWidths, std::set<unsigned int> channels) -> std::vector<AudioFormat>;
};

/**
 * @brief nullFormat
 *
 */
static constexpr inline auto nullFormat = AudioFormat();
} // namespace audio