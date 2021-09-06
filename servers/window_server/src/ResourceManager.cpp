/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "ResourceManager.h"
#include <libg/ImageLoaders/PNGLoader.h>

namespace WinServer {

ResourceManager* s_WinServer_ResourceManager_the = nullptr;

ResourceManager::ResourceManager()
{
    s_WinServer_ResourceManager_the = this;
    LG::PNG::PNGLoader loader;
    m_background = loader.load_from_file("/res/wallpapers/wallpaper.png");
}

} // namespace WinServer