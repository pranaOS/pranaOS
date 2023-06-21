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

#include <cstdint>
#include <array>
#include "equalizer.hpp"

namespace audio::codec {
    enum class Flags {
        OutputMono = 1 << 0,
        OutputStero = 1 << 1,
        InputLeft = 1 << 2,
        InputRight = 1 << 3,
        InputStereo = 1 << 4,
    };

    enum class InputPath {
        Headphones,
        Microphone,
        None
    };

    enum class OutputPath {
        Headphones,
        Earspeaker,
        Loudspeaker,
        None
    };

    struct Configuartion {
        std::uint32_t sampleRate_hz = 0;
        std::uint32_t bitWidth = 0;
        std::uint32_t flags = 0;

        float outputVolume = 0.0f;
        float inputGain = 0.0f;

        std::uint8_t playbackPathGain = 0;
        std::uint8_t playbackPathAtten = 5;

        InputPath inputPath = InputPath::None;
        OutputPath outputPath = OutputPath::None;

    };
}