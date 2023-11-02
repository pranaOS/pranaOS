/**
 * @file device.hpp
 * @author Krisna Pranav
 * @brief device bluetooth
 * @version 6.0
 * @date 2023-11-02
 *
 * @copyright Copyright (c) 2021-2023 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <array>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

struct Device
{
  public:
    static constexpr auto NameBufferSize = 240;

    /**
     * @param name
     */
    explicit Device(std::string name = "")
    {
        setName(name);
    }

    virtual ~Device() = default;

    std::array<char, NameBufferSize> name;

    /**
     * @param name
     */
    void setName(const std::string& name)
    {
    }
};