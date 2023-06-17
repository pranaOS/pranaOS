/**
 * @file commands.hpp
 * @author Krisna Pranav
 * @brief Commands
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <chrono>
#include <vector>
#include <constants.hpp>

namespace Cellular {
    enum class CellularAT{

    };

    enum class commandsSet {
        modemInit,
        simInit,
        smsInit
    };

    std::vector<CellularAT> getCommandsSet(commandsSet set);

    constexpr auto atCmdMaxRetries = 3;
    constexpr auto atCmdRetryDelayMS = 50;
}