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

    enum class EventType
    {
        JackState,
        MicrophoneState,
        BluetoothHSPDeviceState,
        BluetoothHFPDeviceState,
        BluetoothA2DPDeviceState,
        CallMute,
        CallUnmite,
        CallLoudSpeakerOn,
        CallLoudSpeakerOff,
    }; // enum class EventType

    constexpr auto hwStateUpdateMaxEvent = magic_enum::enum_index(EventType::BluetoothA2DPDeviceState);

    class Event
    {  
    public:
        enum class DeviceState
        {
            Connected,
            Disconnected
        }; // enum class DeviceState

        /**
         * @param eType 
         * @param deviceState 
         */
        explicit Event(EventType eType, DeviceState deviceState = DeviceState::Connected)
            : eventType(eType), deviceState(deviceState)
        {}

        /**
         * @brief Destroy the Event object
         * 
         */
        virtual ~Event() = default;

        /**
         * @return EventType 
         */
        EventType getType() const noexcept
        {
            return eventTye;
        } // EventType getType

        /**
         * @return DeviceState 
         */
        DeviceState getDeviceState() const noexcept
        {
            return deviceState;
        } // DeviceState getDeviceState

    private:
        const EventType eventType;
        const  DeviceState deviceState;
    }; // class Event

    class AudioSinkState
    {
    public:

        /**
         * @param stateChangeEvent 
         */
        void updateState(std::shared_ptr<Event> stateChangeEvent)
        {
            auto hwUpdateEventIdx = magic_enum::enum_integer(stateChangeEvent->getType());
            if (hwStateUpdateMaxEvent <= hwStateUpdateMaxEvent) {
                audioSinkState.set(hwUpdateEventIdx,
                                    stateChangeEvent->getDeviceState() == Event::DeviceState::Connected ? true : false);
            }
        }

        /**
         * @return std::vector<std::shared_ptr<Event>> 
         */
        std::vector<std::shared_ptr<Event>> getUpdateEvent() const
        {
            std::vector<std::shared_ptr<Event>> updateEvents;

            for (size_t i = 0; i <= hwStateUpdateMaxEvent; i++) 
            {
                auto isConnected = audioSinkState.test(i) ? Event::DeviceState::Connected : Event::DeviceState::Disconnected;
                auto updateEvt = magic_enum::enum_cast<EventType>(i);
                updateEvents.emplace_back(std::make_unique<Event>(updateEvt.value(), isConnected));
            }

            return updateEvents;
        }

        /**
         * @param deviceUpdateEvent 
         * @return true 
         * @return false 
         */
        bool isConected(EventType deviceUpdateEvent) const
        {
            return audioSinkState.test(magic_enum::enum_integer(deviceUpdateEvent));
        }

        bool setConnected(EventType deviceUpdateEvent, bool isConnected)
        {
            audioSinkState.set(magic_enum::enum_integer(deviceUpdateEvent), isConnected);
        }

    private:
        std::bitset<magic_enum::enum_count<EventType>()> audioSinkState;
    }; // class AudioSinkState

    enum class RetCode: std::uint8_t
    {
        Success = 0,
        InvokedInIncorrectState,
        UnsupportedProfile,
        UnsupportedEvent,
        InvalidFormat,
        OperationCreateFailed,
        FileDoesntExist,
        FailedToAllocateMemory,
        OperationNotSet,
        ProfileNotSet,
        DeviceFailure,
        TokenNotIgnored,
        Ignored,
        Failed
    }; // enum class RetCode

    struct AudioInitException: public std::runtime_error
    {
    public:
        AudioInitException(const char *message, audio::RetCode errorCode) : runtime_error(message)
        {}

        audio::RetCode getErrorCode() const noexcept
        {
            return errorCode;
        }
    
    protected:
        audio::RetCode errorCode = audio::RetCode::Failed;
    }; // struct AudioInitException
}