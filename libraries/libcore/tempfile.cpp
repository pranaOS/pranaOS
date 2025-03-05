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

    /**
     * @param type 
     * @return NonnullOwnPtr<TempFile> 
     */
    NonnullOwnPtr<TempFile> TempFile::create(Type type)
    {
        return adopt_own(*new TempFile(type));
    }

    /**
     * @param type 
     * @return String 
     */
    String TempFile::create_temp(Type type)
    {
        char name_template[] = "/tmp/tmp.XXXXXX";

        switch (type) {
        case Type::File: {
            auto fd = mkstemp(name_template);
            VERIFY(fd >= 0);
            close(fd);
            break;
        }
        case Type::Directory: {
            auto fd = mkdtemp(name_template);
            VERIFY(fd != nullptr);
            break;
        }
        }
        return String { name_template };
    }
    
    /**
     * @brief Construct a new TempFile::TempFile object
     * 
     * @param type 
     */
    TempFile::TempFile(Type type)
        : m_type(type)
        , m_path(create_temp(type))
    {
    }

    /**
     * @brief Destroy the TempFile::TempFile object
     * 
     */
    TempFile::~TempFile()
    {
        File::RecursionMode recursion_allowed { File::RecursionMode::Disallowed };
        if (m_type == Type::Directory)
            recursion_allowed = File::RecursionMode::Allowed;

        auto rc = File::remove(m_path.characters(), recursion_allowed, false);
        if (rc.is_error()) {
            warnln("File::remove failed: {}", rc.error().string_literal());
        }
    }

} // namespace Core
