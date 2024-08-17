/**
 * @file audiocommon.hpp
 * @author Krisna Pranav
 * @brief audio common types
 * @version 6.0
 * @date 2024-08-17
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */


#pragma once

#include "audiodevice.hpp"
#include "profile.hpp"
#include <service/message.hpp>
#include <utils.hpp>
#include <bitset>
#include <map>
#include <memory>
#include <utility>

namespace audio
{
    class AudioMux;
} // namespace audio

namespace audio
{
    inline constexpr Volume defaultVolumeStep = 1;
    inline constexpr Volume maxVolume = 10;
    inline constexpr Volume minVolume = 0;

    inline constexpr Gain maxGain = 100;
    inline constexpr Gain minGain = 0;

    inline constexpr auto audioDbPrefix = "audio";
    inline constexpr auto dbPathSeperator = '/';

    enum class Setting
    {
        Volume,
        Gain,
        EnableVibration,
        VibrationLevel,
        EnableSound,
        Sound,
        IsSystemSound
    }; // enum class Setting

    enum class SettingState : bool
    {
        Enabled,
        Disabled
    }; // enum class SettingState

    enum class PlaybackType
    {
        
    }; // enum class PlaybackType
}