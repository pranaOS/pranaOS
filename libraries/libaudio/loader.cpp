/**
 * @file loader.cpp
 * @author Krisna Pranav
 * @brief Loader
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <LibAudio/FlacLoader.h>
#include <LibAudio/Loader.h>
#include <LibAudio/MP3Loader.h>
#include <LibAudio/WavLoader.h>

namespace Audio 
{
    /**
     * @brief Construct a new Loader::Loader object
     * 
     * @param plugin 
     */
    Loader::Loader(NonnullOwnPtr<LoaderPlugin> plugin)
        : m_plugin(move(plugin))
    {
    }   
    
    /**
     * @param path 
     * @return Result<NonnullOwnPtr<LoaderPlugin>, LoaderError> 
     */
    Result<NonnullOwnPtr<LoaderPlugin>, LoaderError> Loader::try_create(StringView path)
    {
        NonnullOwnPtr<LoaderPlugin> plugin = adopt_own(*new WavLoaderPlugin(path));
        auto initstate0 = plugin->initialize();

        if (!initstate0.is_error())
            return plugin;

        plugin = adopt_own(*new FlacLoaderPlugin(path));
        auto initstate1 = plugin->initialize();

        if (!initstate1.is_error())
            return plugin;

        plugin = adopt_own(*new MP3LoaderPlugin(path));
        auto initstate2 = plugin->initialize();

        if (!initstate2.is_error())
            return plugin;

        return LoaderError { "No loader plugin available" };
    }

    /**
     * @param buffer 
     * @return Result<NonnullOwnPtr<LoaderPlugin>, LoaderError> 
     */
    Result<NonnullOwnPtr<LoaderPlugin>, LoaderError> Loader::try_create(Bytes& buffer)
    {
        NonnullOwnPtr<LoaderPlugin> plugin = adopt_own(*new WavLoaderPlugin(buffer));

        if (auto initstate = plugin->initialize(); !initstate.is_error())
            return plugin;

        plugin = adopt_own(*new FlacLoaderPlugin(buffer));

        if (auto initstate = plugin->initialize(); !initstate.is_error())
            return plugin;

        return LoaderError { "No loader plugin available" };
    }

} // namespace Audio
