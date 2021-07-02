/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <LibAudio/WavLoader.h>

namespace Audio {

Loader::Loader(const StringView& path)
{
    m_plugin = make<WavLoaderPlugin>(path);
    if (m_plugin->sniff())
        return;
    m_plugin = nullptr;
}

Loader::Loader(const ByteBuffer& buffer)
{
    m_plugin = make<WavLoaderPlugin>(buffer);
    if (m_plugin->sniff())
        return;
    m_plugin = nullptr;
}

}