/**
 * @file common.hpp
 * @author Krisna Pranav
 * @brief Common
 * @version 1.0
 * @date 2023-06-20
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "message.hpp"
#include "../phone.hpp
#include "../tethering.hpp"

namespace sys::phonemodes
{

class PhoneModeChanged : public DataMessage
{
  public:
    /**
     * @brief Construct a new Phone Mode Changed object
     *
     * @param phoneMode
     */
    explicit PhoneModeChanged(PhoneMode phoneMode)
        : DataMessage{MessageType::MessageTypeUninitialized}, phoneMode{phoneMode}
    {
    }

    /**
     * @brief Get the Phone Mode object
     *
     * @return auto
     */
    [[nodiscard]] auto getPhoneMode() const noexcept
    {
        return phoneMode;
    }

  private:
    PhoneMode phoneMode;
};

class TetheringChanged : public DataMessage
{
  public:
    /**
     * @brief Construct a new Tethering Changed object
     *
     * @param tetheringMode
     */
    explicit TetheringChanged(Tethering tetheringMode)
        : DataMessage{MessageType::MessageTypeUninitialized}, tethering{tetheringMode}
    {
    }

    /**
     * @brief Get the Tethering Mode object
     *
     * @return auto
     */
    [[nodiscard]] auto getTetheringMode() const noexcept
    {
        return tethering;
    }

  private:
    Tethering tethering;
};

class ChangedSuccessfully : public ResponseMessage
{
};

class ChangeFailed : public ResponseMessage
{
};
} // namespace sys::phonemodes