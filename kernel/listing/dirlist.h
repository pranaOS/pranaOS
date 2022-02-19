/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <listing/control.h>

namespace Kernel {
    class dirList : public controller {
    public:
        dirList();

        /**
         * @brief begin the dir listing
         * 
         * @param thread 
         * @param pathPtr 
         * @return int 
         */
        int beginListing(Thread *thread, ak::uint32_t pathPtr) override;

    }
}