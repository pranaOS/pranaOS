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
       None,
       Multimedia,
       Notifications,
       System = Notifications,
       SingleVibration = Notifications,
       KeypadSound,
       CallRingtone,
       TextMessageRingtone,
       Meditation,
       Alarm,
       PreWakeUp,
       Snooze,
       FocusTimer,
       Bedtime,
       Last = Bedtime, 
    }; // enum class PlaybackType

    enum class VolumeChangeRequestSource
    {
        A2DP,
        HFP,
        HSP,
        Other
    }; // enum class VolumeChangeRequestSource

    enum class Fade
    {
        Disable,
        In,
        InOut
    }; // enum class Fade

    struct FadeParams
    {
        Fade mode;
        std::optional<std::chrono::seconds> playbackDuraiton = std::nullopt;
    }; // struct FadeParams

    enum class VolumeUpdateType
    {
        UpdateDB,
        SkipUpdateDB
    }; // enum class VolumeUpdateType

    using Context = std::pair<Profile::Type, PlaybackType>;

    struct DbPathElement
    {
        Setting setting;
        PlaybackType playbackType;
        Profile::Type profileType;
    }; // struct DbPathElement

    /**
     * @param playbackType 
     * @return const std::string 
     */
    [[nodiscard]] const std::string str(const PlaybackType &playbackType) noexcept;

    /**
     * @param setting 
     * @return const std::string 
     */
    [[nodiscard]] const std::string str(const Setting &setting) noexcept;

    /**
     * @param element 
     * @return const std::string 
     */
    [[nodiscard]] const std::string dbPath(const DbPathElement &element);

    /**
     * @param setting 
     * @param playbackType 
     * @param profileType 
     * @return const std::string 
     */
    [[nodiscard]] const std::string dbPath(const Setting &setting, const PlaybackType &playbackType, const Profile::Type &profileType);
}