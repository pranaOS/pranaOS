/**
 * @file message_forward.hpp
 * @author Krisna Pranav
 * @brief Message Forward
 * @version 6.0
 * @date 2023-06-20
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <memory>
#include "../sys_return_codes.hpp"

namespace sys {
    class Service;
    class Message;

    using MessagePointer = std::shared_ptr<Message>;
    using MessageNone = std::nullptr_t;
    using SendResult     = std::pair<ReturnCodes, std::shared_ptr<Message>>;
}