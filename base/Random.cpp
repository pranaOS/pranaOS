/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
/*
 * Copyright (c) 2021, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Random.h>

namespace Base {

u32 get_random_uniform(u32 max_bounds)
{
    const u32 max_usable = UINT32_MAX - (static_cast<u64>(UINT32_MAX) + 1) % max_bounds;
    auto random_value = get_random<u32>();
    for (int i = 0; i < 20 && random_value > max_usable; ++i) {

        random_value = get_random<u32>();
    }
    return random_value % max_bounds;
}

}