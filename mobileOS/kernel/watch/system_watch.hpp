/**
* @file system_watch.hpp
* @author Krisna Pranav
* @brief System Watch Functionalities
* @version 1.0
* @date 2023-06-20
*
* @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "watch.hpp"

namespace sys {
    class SystemWatch : public Watch {
      public:
        static constexpr auto threadName = "SystemWatch";

        SystemWatch(const SystemWatch&) = delete;
        SystemWatch(SystemWatch &&) = delete;
        SystemWatch &operator=(const SystemWatch &) = delete;
        SystemWatch &operator=(SystemWatch &&) = delete;

        static SystemWatch &getInstance();

        bool init();

        void refresh() override;

      private:
        SystemWatch();

        static constexpr TickType_t refreshTimeoutPreiod = pdMS_TO_TICKS(9000);

        void Run() final;
    };
}