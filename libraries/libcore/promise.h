/**
 * @file promise.h
 * @author Krisna Pranav
 * @brief promise
 * @version 6.0
 * @date 2025-03-02
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libcore/eventloop.h>
#include <libcore/object.h>

namespace Core
{

    template<typename Result>
    class Promise : public Object
    {
        C_OBJECT(Promise);

    public:
        /**
         * @param result 
         */
        void resolve(Result&& result)
        {
        }

    private:
        Promise() = default;

        Optional<Result> m_pending;
    }; // class Promise : public Object

} // namespace Core
