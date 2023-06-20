/**
* @file common.hpp
* @author Krisna Pranav
* @brief Common
* @version 1.0
* @date 2023-06-20
*
* @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "message.hpp"
#include "../phone.hpp
#include "../tethering.hpp"

namespace sys::phonemodes {
    class PhoneModeChanged : public DataMessage {
      public:
        explicit PhoneModeChanged(PhoneMode phoneMode) : DataMessage{MessageType::MessageTypeUninitialized}, phoneMode{phoneMode}
        {}

        [[nodiscard]] auto getPhoneMode() const noexcept {
            return phoneMode;
        }

      private:
        PhoneMode phoneMode;
    };

    class ChangedSuccessfully : public ResponseMessage {};

    class ChangeFailed : public ResponseMessage {};
}