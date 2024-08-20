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

    class Token
    {
        using TokenType = int16_t;

    public:
        /**
         * @param initValue 
         */
        explicit Token(TokenType initValue = tokenUninitialzied) : t(initValue)
        {}

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const Token &other) const noexcept
        {
            return other.t == t;
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const Token &other) const noexcept
        {
            return !(other.t == t);
        }

        /**
         * @return true 
         * @return false 
         */
        bool IsValid() const
        {
            return t > tokenUninitialzied;
        }

        /**
         * @return true 
         * @return false 
         */
        bool IsBad() const
        {
            return t == tokenBad;
        }

        bool IsUninitialized() const
        {
            return t == tokenUninitialzied;
        }

        static inline MakeBadToken()
        {
            return Token(tokenBad);
        }

    private:
        static constexpr auto maxToken = std::numeric_limits<TokenType>::max();

        Token IncrementToken()
        {
            t = (t == maxToken) ? 0 : t + 1;
            return *this;
        }

        constexpr static TokenType tokenUninitialzied = -1;
        constexpr static TokenType tokenBad = -2;

        TokenType t;
        friend class ::audio::AudioMux;
    };

    /**
     * @param retCode 
     * @return RetCode 
     */
    RetCode GetDeviceError(AudioDevice::RetCode retCode);

    /**
     * @param retcode 
     * @return const std::string 
     */
    const std::string str(RetCode retcode);

    /**
     * @param volume 
     * @return std::string 
     */
    [[nodiscard]] auto GetVolumeText(const audio::Volume &volume) -> std::string;
} // namespace audio

namespace AudioServiceMessage
{
    class EndOfFile: public sys::DataMessage
    {
    public:
        explicit EndOfFile(audio::Token &token) : token(token)
        {}

        const audio::Token &GetToken() const
        {
            return token;
        }

    private:
        audio::Token token = audio::Token::MakeBadToken();
    }; // class EndOfFile

    class FileDeleted: public sys::DataMessage
    {
    public:
        explicit FileDeleted(audio::Token &token) : token(token)
        {}

        const audio::Token &GetToken() const
        {
            return token;
        }

    private:
        audio::Token token = audio::Token::MakeBadToken();
    }; // class FileDeleted 

    class FileSystemNoSpace: public sys::DataMessage
    {
    public:
        explicit FileSystemNoSpace(audio::Token &token) : token(token)
        {}

        const audio::Token &GetToken() const
        {
            return token;
        }

    private:
        audio::Token token = audio::Token::MakeBadToken();
    }; // class FileSystemNoSpace
    
    class DbRequest: public sys::DataMessage
    {
    public:
        /**
         * @param setting 
         * @param playback 
         * @param profile 
         */
        explicit DbRequest(const audio::Setting &setting, const audio::PlaybackType &playback,
                           const audio::Profile::Type &profile)
            : setting(setting), profile(profile), playback(playback)
        {}

        const audio::Setting setting;
        const audio::Profile::Type profile;
        const audio::PlaybackType playback;
    }; // class DbRequest
} // namespace AudioServiceMessage