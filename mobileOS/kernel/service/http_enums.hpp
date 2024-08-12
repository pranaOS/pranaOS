/**
 * @file http_enums.hpp
 * @author Krisna Pranav
 * @brief http enums
 * @version 6.0
 * @date 2024-08-12
 *
 * @copyright Copyright (c) 2021 - 2024 Krisna Pranav, pranaOS Developers
 *
 */

#pragma once

#include <cstdint>

namespace sdesktop::endpoints::http
{
    enum class Code
    {
        OK = 200,
        Created = 201,
        Accepted = 202,
        NoContent = 204,
        SeeOther = 303,
        BadRequest = 400,
        Forbidden = 403,
        NotFound = 404,
        NotAcceptable = 406,
        Conflict = 409,
        UnprocessableEntity = 422,
        Locked = 423,
        InternalServerError = 500,
        NotImplemented = 501,
        InsufficientStorage = 507
    }; // enum class Code

    enum class Method
    {
        get = 1,
        post,
        put,
        del
    }; // enum class Method

    [[nodiscard]] auto isMethodValid(uint8_t) -> bool;
} // namespace sdesktop::endpoints::http