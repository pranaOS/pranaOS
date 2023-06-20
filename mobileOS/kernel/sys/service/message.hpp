/**
* @file message.hpp
* @author Krisna Pranav
* @brief Message Functionalities
* @version 1.0
* @date 2023-06-20
*
* @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
*
*/

#pragma once

#include "message_forward.hpp"
#include "../common.hpp"
#include "../message_type.hpp"
#include <magic_enum.hpp>
#include <cstdint>
#include <memory>
#include <string>

namespace sys {
    SendResult createSendResult(ReturnCodes retCode, MessagePointer msg);
    inline constexpr std::uint64_t invalidMessageUID = std::numeric_limits<uint64_t>().max();

    using MessageUIDType = std::uint64_t;

    class MessageUID {
    protected:
      MessageUIDType value = 0;

    public:
      [[nodiscard]] MessageUIDType get() const noexcept;
      [[nodiscard]] MessageType getNext() const noexcept;
    };

    class Message {
    public:
      enum class TransmissionType {
          Unspecified,
          Unicast,
          Multicast,
          Broadcast
      };

      enum class Type {
          Unspecified,
          System,
          Data,
          Response
      };

      explicit Message(Type type);
      Message(Type type, BusChannel channel);

      virtual ~Message() noexcept = default;

      virtual MessagePointer Execute(Service *service);

      virtual explicit operator std::string() const {
          return {"{}"};
      }
    };

    class DataMessage : public Message {
    public:
      explicit DataMessage(MessageType messageType);
      explicit DataMessage(BusChannel channel);
      DataMessage();

      MessageType messageType = MessageType::MessageTypeUninitialized;
    };

    class ReadyToCloseMessage : public DataMessage {};

    class ResponseMessage : public Message {
    public:
      explicit ResponseMessage(ReturnCodes retCode = ReturnCodes::Success, MessageType responseInfo = MessageType::MessageTypeUninitialized);
      MessagePointer execute(Service *service) final;

      ReturnCodes retCodes;
      MessageType responseTo;
    };

    inline auto msgHandled() -> MessagePointer {
      return std::make_shared<ResponseMessage>();
    }

    inline auto msgNotHandlded() -> MessagePointer {
      return std::make_shared<ResponseMessage>(ReturnCodes::Unresolved);
    }
}