/**
 * @file volumescaler.hpp
 * @author Krisna Pranav
 * @brief Volume Scalling(0-100)
 * @version 6.0
 * @date 2024-08-21
 * 
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "audiocommon.hpp"

namespace audio::volume::scaler
{
    namespace a2dp
    {
        /**
         * @param avrcpVolume 
         * @return Volume 
         */
        Volume toSystemVolume(std::uint8_t avrcpVolume) noexcept;
    } // namespace a2dp

    namespace hsp
    {
        /**
         * @param hspSpeakerGain 
         * @return Volume 
         */
        Volume toSystemVolume(std::uint8_t hspSpeakerGain) noexcept;

        std::uint8_t toHSPGain(float systemVolume) noexcept;
    } // namespace hsp

    namespace hfp
    {
        /**
         * @param hfpSpeakerGain 
         * @return Volume 
         */
        Volume toSystemVolume(std::uint8_t hfpSpeakerGain) noexcept;

        std::uint8_t toHFPGain(float systemVolume) noexcept;
    }; // namespace hfp 
    
} // namespace audio::volume::scaler 