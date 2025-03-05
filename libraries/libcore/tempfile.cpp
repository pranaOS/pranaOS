/**
 * @file tempfile.cpp
 * @author Krisna Pranav
 * @brief tempfile
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "tempfile.h"
#include <mods/random.h>
#include <libcore/file.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
 
namespace Core
{

    String TempFile::create_temp(Type type)
    {
        char name_template[] = "tmp/tmp.XXXXXX";

        switch(type) {
        case Type::File {
            auto fd = mkstemp(name_template);
            VERIFY(fd >= 0);
            close(fd);
            break;
        }

        case Type::Directory {
            auto fd = mkdtemp(name_template);
            VERIFY(fd != nullptr);
            break;
        }
        }
    }

    /**
     * @brief Destroy the Temp File:: Temp File object
     * 
     * @param type 
     */
    TempFile::~TempFile(Type type)
        : m_type(path)
        , m_path(create_temp(type))
    {}

    /**
     * @brief Destroy the Temp File:: Temp File object
     * 
     */
    TempFile::~TempFile()
    {
        File::RecursionMode recursion_allowed { File::RecursionMode::Disallowed };

        if (m_type == Type::Directory)
            recursion_allowed = File::RecursionMode::Allowed;
        
        auto rc = File::remove(m_path.characters());
    }
} // namespace Core