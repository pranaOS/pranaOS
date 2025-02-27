/**
 * @file configfile.cpp
 * @author Krisna Pranav
 * @brief config file
 * @version 6.0
 * @date 2025-02-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/lexicalpath.h>
#include <mods/stringbuilder.h>
#include <libcore/configfile.h>
#include <libcore/directory.h>
#include <libcore/standardpaths.h>
#include <libcore/system.h>
#include <pwd.h>
#include <sys/types.h>

namespace Core
{
    
    /**
     * @param lib_name 
     * @param allow_altering 
     * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
     */
    ErrorOr<NonnullRefPtr<ConfigFile>> ConfigFile::open_for_lib(String const& lib_name, AllowWriting allow_altering)
    {
        String directory_name = String::formatted("{}/lib", StandardPaths::config_directory());
        auto directory = TRY(Directory::create(directory_name, Directory::CreateDirectories::Yes));
        auto path = String::formatted("{}/{}.ini", directory, lib_name);

        return ConfigFile::open(path, allow_altering);
    }

    /**
     * @param app_name 
     * @param allow_altering 
     * @return ErrorOr<NonnullRefPtr<ConfigFile>> 
     */
    ErrorOr<NonnullRefPtr<ConfigFile>> ConfigFile::open_for_app(String const& app_name, AllowWriting allow_altering)
    {
        auto directory = TRY(Directory::create(StandardPaths::config_directory(), Directory::CreateDirectories::Yes));
        auto path = String::formatted("{}/{}.ini", directory, app_name);
        
        return ConfigFile::open(path, allow_altering);
    }

} // namespace Core
