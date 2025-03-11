/**
 * @file version.cpp
 * @author Krisna Pranav
 * @brief version
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libcore/configfile.h>
#include <libcore/version.h>

namespace Core::Version 
{
    
    /**
     * @return String 
     */
    String read_long_version_string()
    {
        auto version_config = Core::ConfigFile::open("/res/version.ini").release_value_but_fixme_should_propagate_errors();
        auto major_version = version_config->read_entry("Version", "Major", "0");
        auto minor_version = version_config->read_entry("Version", "Minor", "0");

        StringBuilder builder;
        builder.appendff("Version {}.{}", major_version, minor_version);
        if (auto git_version = version_config->read_entry("Version", "Git", ""); git_version != "")
            builder.appendff(".g{}", git_version);
        return builder.to_string();
    }

} // namespace Core::Version 
