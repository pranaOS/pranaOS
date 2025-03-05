/**
 * @file inspector.h
 * @author Krisna Pranav
 * @brief inspector
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include "reader.h"
#include <mods/noncopyable.h>
#include <libdebug/processinspector>

namespace Coredump
{

    class Inspector : public Debug::ProcessInspector
    {
    private:
        void parse_loaded_libraries(Function<void(float)> on_progress);

        size_t number_of_libraries_in_coredump() const;

        NonnullOwnPtr<Reader> m_reader;

        NonnullOwnPtrVector<Debug::LoadedLibrary> m_loaded_libraries;
    }; // class Inspector : public Debug::ProcessInspector

} // namespace Coredump
