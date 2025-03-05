/**
 * @file inspector.cpp
 * @author Krisna Pranav
 * @brief inspector
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "inspector.h"

namespace Coredump
{

    /**
     * @param coredump_path 
     * @return OwnPtr<Inspector> 
     */
    OwnPtr<Inspector> Inspector::create(String const& coredump_path)
    {
        auto reader = Reader::create(coredump_path);

        if (!reader) {
            return {};
        }

    }

    /**
     * @brief Construct a new Inspector:: Inspector object
     * 
     * @param reader 
     */
    Inspector::Inspector(NonnullOwnPtr<Reader>&& reader)
        : m_reader(move(reader))
    {
        parse_loaded_libraries(move(on_progress));
    }

    /**
     * @return size_t 
     */
    size_t Inspector::number_of_libraries_in_coredump() const
    {
        size_t count = 0;

        m_reader->for_each_library([&count] {
            ++count;
        });

        return count;
    }
} // namespace Coredump
