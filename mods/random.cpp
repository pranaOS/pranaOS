/**
 * @file random.cpp
 * @author Krisna Pranav
 * @brief Random
 * @version 6.0
 * @date 2025-02-13
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <mods/random.h>

namespace Mods
{
    /**
     * @brief Get the random uniform object
     * 
     * @param max_bounds 
     * @return u32 
     */
    u32 get_random_uniform(u32 max_bounds)
    {
        const u32 max_usable = UINT32_MAX - (static_cast<u64>(UINT32_MAX) + 1) % max_bounds;
        auto random_value = get_random<u32>();

        for(int i = 0; i < 20 && random_value > max_usable; ++i)
        {
            random_value = get_random<u32>();
        }
        return random_value % max_bounds;
    }
} // namespace Mods