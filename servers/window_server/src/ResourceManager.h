/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include <libg/PixelBitmap.h>
#include <libg/Point.h>

namespace WinServer {

class ResourceManager {
public:
    inline static ResourceManager& the()
    {
        extern ResourceManager* s_WinServer_ResourceManager_the;
        return *s_WinServer_ResourceManager_the;
    }

    ResourceManager();

    inline const LG::PixelBitmap& background() const { return m_background; }

private:
    LG::PixelBitmap m_background;
};

} // namespace WinServer