/**
 * @file urc.hpp
 * @author Krisna Pranav
 * @brief URC
 * @version 1.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libutils/utils.hpp>
#include "urc_handler.hpp"
    
class Cellular::URC {
    class URC {
    public:
        /**
         * @brief Construct a new URC object
         * 
         * @param urcBody 
         * @param urcHead 
         */
        URC(const std::string &urcBody, const std::string &urcHead = std::string());

        virtual ~URC() = default;

        /**
         * @brief isValid
         * 
         * @return true 
         * @return false 
         */
        virtual auto isValid() const noexcept -> bool {
            return !urcBody.empty() || !urcHead.empty();
        }

        /**
         * @brief Get the Tokens object
         * 
         * @return std::vector<std::string> 
         */
        auto getTokens() const -> std::vector<std::string>;

        auto getURCBody() const -> std::string {
            return urcBody;
        }
    };
};