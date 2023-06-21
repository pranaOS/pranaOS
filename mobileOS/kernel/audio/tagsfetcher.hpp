/**
* @file tagsfetcher.hpp
* @author Krisna Pranav
* @brief Tags Fetcher
* @version 1.0
* @date 2023-06-20
*
* @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

namespace tags::fetcher {
    struct Tags {
        uint32_t total_duration_s = 0;
        uint32_t duration_hour = 0;
        uint32_t duration_min = 0;
        uint32_t duration_sec = 0;

        uint32_t sample_rate = 0;
        uint32_t num_channel = 0;
        uint32_t bitrate = 0;

        Tags() = default;

        Tags(uint32_t total_duration_s);
    };

    Tags fetchTags(const std::string &fileName);
}