/**
* @file watch.hpp
* @author Krisna Pranav
* @brief Watch
* @version 1.0
* @date 2023-06-20
*
* @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
*
 */

#pragma once

namespace sys {
    class Watch {
      public:
        virtual void refresh() = 0;
    };
}