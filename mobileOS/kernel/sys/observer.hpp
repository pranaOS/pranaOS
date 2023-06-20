/**
* @file message_type.hpp
* @author Krisna Pranav
* @brief Message Type
* @version 1.0
* @date 2023-06-20
*
* @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include <functional>
#include <stdexcept>
#include "common.hpp"
#include "service/common.hpp"

namespace sys::phonemodes {
    class Observer {
      public:
        using OnPhoneModeChangedCallback = std::function<void(PhoneMode)>;
        using OnTetheringChangedCallback = std::function<void(Tethering)>;

        void connect(Service *owner);
        void subscribe(OnPhoneModeChangedCallback &&onChange) noexcept;
        void subscribe(OnTetheringChangedCallback &&onChange) noexcept;

        bool isInMode(PhoneMode mode) const noexcept;
        PhoneMode getCurrentPhoneMode() const noexcept;
        bool isTetheringOn() const noexcept;

      private:
        sys::MessagePointer handlePhoneModeChange(PhoneModeChanged *message);
        sys::MessagePointer handleTetheringChange(TetheringChanged *message);

        OnPhoneChangedCallback onPhoneChangedCallback;
        OnTetheringChangedCallback onTetheringChangedCallback;
        PhoneMode phoneMode = PhoneMode::Uninitialized;
        Tethering tetheringMode = Tethering::Off;
    };
}

namespace sys {
    class Service;
}