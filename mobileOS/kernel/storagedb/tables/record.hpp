/**
 * @file record.hpp
 * @author Krisna Pranav
 * @brief Record Database
 * @version 6.0
 * @date 2023-06-17
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <cstdint>
#include <log/log.hpp>

constexpr uint32_t DB_ID_NONE = 0;

struct Record
{
    uint32_t ID = DB_ID_NONE;

    Record() = default;
    Record(uint32_t ID)
        : ID(ID)
    {
    }

    bool isValid() const
    {
        auto result = ID != DB_ID_NONE;
        if(!result)
        {
            LOG_DEBUG("RECORD VALIDATION FAILED - INCORRECT ID");
        }

        return result;
    }
};