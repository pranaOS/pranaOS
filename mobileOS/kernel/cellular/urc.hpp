/**
 * @file urc.hpp
 * @author Krisna Pranav
 * @brief URC
 * @version 6.0
 * @date 2023-06-17
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "urc_handler.hpp"
#include <libutils/utils.hpp>

namespace Cellular::URC
{
class URC
{
  public:
    /**
     * @brief Construct a new URC object
     *
     * @param urcBody
     * @param urcHead
     */
    URC(const std::string& urcBody, const std::string& urcHead = std::string());

    virtual ~URC() = default;

    /**
     * @brief isValid
     *
     * @return true
     * @return false
     */
    virtual auto isValid() const noexcept -> bool
    {
        return !urcBody.empty() || !urcHead.empty();
    }

    /**
     * @brief Get the Tokens object
     *
     * @return std::vector<std::string>
     */
    auto getTokens() const -> std::vector<std::string>;

    /**
     * @brief Get the Urc Body object
     *
     * @return std::string
     */
    auto getURCBody() const -> std::string
    {
        return urcBody;
    }

    virtual void Handle(UrcHandler& h)
    {
    }

    /**
     * @brief Set the Handled object
     *
     * @param state
     */
    void setHandled(bool state)
    {
        isUrcHandled = state;
    }

    /**
     * @brief isHandled
     *
     * @return true
     * @return false
     */
    bool isHandled()
    {
        return isUrcHandled;
    }

  protected:
    std::vector<std::string> tokens;
    std::string urcBody;
    std::string urcHead;

    bool isUrcHandled = false;

    /**
     * @brief split
     *
     * @param str
     */
    virtual void split(const std::string& str);
};

} // namespace Cellular::URC