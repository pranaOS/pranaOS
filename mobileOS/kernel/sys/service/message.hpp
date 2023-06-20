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

    /**
     * @brief Create a Send Result object
     * 
     * @param retCode 
     * @param msg 
     * @return SendResult 
     */
    SendResult createSendResult(ReturnCodes retCode, MessagePointer msg);

    /**
     * @brief invalidMessageUID
     * 
     */
    inline constexpr std::uint64_t invalidMessageUid = std::numeric_limits<uint64_t>().max();

    using MessageUIDType = std::uint64_t;

    class MessageUID {
    protected:
        MessageUIDType value = 0;

    public:
        /**
         * @brief MessageUIDType(get)
         * 
         * @return MessageUIDType 
         */
        [[nodiscard]] MessageUIDType get() const noexcept;
        
        /**
         * @brief Get the Next object
         * 
         * @return MessageUIDType 
         */
        [[nodiscard]] MessageUIDType getNext() noexcept;
    };

    class Message {
    public:
        /**
         * @brief TransmissionType
         * 
         */
        enum class TransmissionType {
            Unspecified,
            Unicast,
            Multicast,
            Broadcast
        };

        /**
         * @brief Type
         * 
         */
        enum class Type {
            Unspecified,
            System,
            Data,
            Response
        };

        /**
         * @brief Construct a new Message objec
         * 
         * @param type 
         */
        explicit Message(Type type);
        Message(Type type, BusChannel channel);
        virtual ~Message() noexcept = default;

        virtual MessagePointer Execute(Service *service);

        virtual explicit operator std::string() const {
            return {"{}"};
        }

        MessageUIDType id          = invalidMessageUid;
        MessageUIDType uniID       = invalidMessageUid;
        Type type                  = Type::Unspecified;
        TransmissionType transType = TransmissionType::Unspecified;
        BusChannel channel         = BusChannel::Unknown;
        std::string sender         = "Unknown";

        [[nodiscard]] std::string to_string() const
        {
            return "| ID:" + std::to_string(id) + " | uniID: " + std::to_string(uniID) +
                   " | Type: " + std::string(magic_enum::enum_name(type)) +
                   " | TransmissionType: " + std::string(magic_enum::enum_name(transType)) +
                   " | Channel: " + std::string(magic_enum::enum_name(channel)) + " | Sender: " + sender + " |";
        }

        [[nodiscard]] bool ValidateMessage() const noexcept;

        void ValidateResponseMessage() const;
        void ValidateUnicastMessage() const;
        void ValidateBroadcastMessage() const;
        void ValidateMulticastMessage() const;
    };

    namespace msg {
        struct Request : public sys::Message {
            Request() : Message(Type::Data)
            {}
            virtual const char *target() const = 0;
        };

        struct Response : public sys::Message {
            explicit Response(bool retCode = true) : Message(Type::Data), retCode(retCode)
            {}
            const bool retCode;
        };

        struct Notification : public sys::Message {
            Notification() : Message(Type::Data)
            {}
            virtual sys::BusChannel channel() const = 0;
        };
    }

    enum class SystemMessageType {
        SwitchPowerMode,
        Start,
        Timer,
        Exit,
        ServiceCloseReason
    };

    class SystemMessage : public Message {
    public:
        /**
         * @brief Construct a new System Message object
         * 
         * @param systemMessageType 
         * @param pwrMode 
         */
        explicit SystemMessage(SystemMessageType systemMessageType, ServicePowerMode pwrMode = ServicePowerMode::Active);

        MessagePointer Execute(Service *service) final;

        SystemMessageType systemMessageType;
        ServicePowerMode powerMode;
    };

    class ServiceCloseReasonMessage : public SystemMessage {
    public:
        explicit ServiceCloseReasonMessage(CloseReason closeReason);

        [[nodiscard]] CloseReason getCloseReason() const noexcept;

    private:
        const CloseReason closeReason;
    };

    class DataMessage : public Message {
    public:
        explicit DataMessage(MessageType messageType);
        explicit DataMessage(BusChannel channel);
        DataMessage();

        MessageType messageType = MessageType::MessageTypeUninitialized;
    };

    class ReadyToCloseMessage : public DataMessage
    {};

    class ResponseMessage : public Message {
    public:
        /**
         * @brief Construct a new Response Message object
         * 
         * @param retCode 
         * @param responseTo 
         */
        explicit ResponseMessage(ReturnCodes retCode    = ReturnCodes::Success, MessageType responseTo = MessageType::MessageTypeUninitialized);

        MessagePointer Execute(Service *service) final;

        ReturnCodes retCode;
        MessageType responseTo;
    };

    /**
     * @brief msgHandled
     * 
     * @return MessagePointer 
     */
    inline auto msgHandled() -> MessagePointer {
        return std::make_shared<ResponseMessage>();
    }

    /**
     * @brief msgNotHandlded
     * 
     * @return MessagePointer 
     */
    inline auto msgNotHandled() -> MessagePointer {
        return std::make_shared<ResponseMessage>(ReturnCodes::Unresolved);
    }
} 