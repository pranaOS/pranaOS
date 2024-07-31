//
//  audiodevice.hpp
//  pranaOS
//
//  Created by Krisna Pranav on 31/07/24.
//

#pragma once

#include "endpoint.hpp"
#include <optional>
#include <memory>
#include <functional>

namespace audio
{
    class AudioDevice: public audio::IOProxy
    {
    public:
        static constexpr auto minVolume = .0f;
        static constexpr auto maxVolume = 10.0f;
        
        enum class RetCode
        {
            Success = 0,
            Failure,
            Disconnected
        }; // enum class RetCode
        
        enum class Type
        {
            None,
            Audiocodec,
            Cellular,
            BluetoothA2DP,
            BluetoothHSP,
            BluetoothHFP
        }; // enum class Type
        
        virtual ~AudioDevice() = default;
        
        virtual RetCode Start()
        {
            return RetCode::Success;
        }
        
        virtual RetCode Stop()
        {
            return RetCode::Success;
        }
        
        virtual RetCode Pause()
        {
            return RetCode::Success;
        }
        
        virtual RetCode Resume()
        {
            return RetCode::Success;
        }
        
        virtual RetCode setOutputVolume(float vol) = 0;
        
        virtual RetCode setInputGain(float gain) = 0;
    }; // class AudioDevice
} // namespace Audio
