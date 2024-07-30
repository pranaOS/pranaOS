/**
 * @file bus.hpp
 * @author Krisna Pranav
 * @brief Bus
 * @version 1.0
 * @date 2023-06-20
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "../common.hpp"
#include "../message.hpp"
#include "common.hpp"
#include <cstdint>
#include <memory>
#include <string>

namespace sys
{
class Service;

class Bus
{
    friend class BusProxy;

  private:
    bool sendUnicast(std::shared_ptr<Message> message, const std::string& targetName, Service* sender);

    SendResult sendUnicastSync(std::shared_ptr<Message> message, const std::string& targetName, Service* sender, std::uint32_t timeout);

    SendResult unicastSync(const std::shared_ptr<Message>& message, Service* sender, std::uint32_t timeout);

    void sendMulticast(std::shared_ptr<Message> message, BusChannel channel, Service* sender);

    void sendBroadcast(std::shared_ptr<Message> message, Service* service);

    void sendResponse(std::shared_ptr<Message> response, std::shared_ptr<Message> request, Service* sender);

    static void add(Service* service);

    static void Remove(Service* service);
};
} // namespace sys