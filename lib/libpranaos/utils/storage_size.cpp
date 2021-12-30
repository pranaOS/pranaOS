#include "libpranaos/utils/storage_size.h"
#include <cstdlib>
#include <cstdio>

#define KB_SIZE (1024L)
#define MB_SIZE (KB_SIZE * KB_SIZE)
#define GB_SIZE (MB_SIZE * KB_SIZE)
#define TB_SIZE (GB_SIZE * KB_SIZE)
#define PB_SIZE (TB_SIZE * KB_SIZE)

namespace utils
{
    std::string format_storage_size(uint64_t size_in_bytes)
    {
        std::string res;

        const char* units[] = { "B", "KiB", "MiB", "GiB", "TiB", "PiB" };

        bool(*is[])(uint64_t) =
        { 
            [](uint64_t value) { return value < KB_SIZE; }, 
            [](uint64_t value) { return value >= KB_SIZE && value < MB_SIZE; },
            [](uint64_t value) { return value >= MB_SIZE && value < GB_SIZE; },
            [](uint64_t value) { return value >= GB_SIZE && value < TB_SIZE; },
            [](uint64_t value) { return value >= TB_SIZE && value < PB_SIZE; },
            [](uint64_t value) { return value >= PB_SIZE; }
        };

        uint64_t(*convert[])(uint64_t) =
        { 
            [](uint64_t value) { return value; }, 
            [](uint64_t value) { return value / KB_SIZE; },
            [](uint64_t value) { return value / MB_SIZE; },
            [](uint64_t value) { return value / GB_SIZE; },
            [](uint64_t value) { return value / TB_SIZE; },
            [](uint64_t value) { return value / PB_SIZE; }
        };

        for(int i = 0; i < 6; i++)
        {
            if(is[i](size_in_bytes))
            {
                res.append(std::to_string(convert[i](size_in_bytes))).append(units[i]);

                break;
            }
        }

        return std::string(res);
    }
}