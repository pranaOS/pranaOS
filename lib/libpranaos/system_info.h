#pragma once

#include <string>

namespace pranaos
{
    struct system_info
    {
        std::string os_name;
        std::string os_version;
        std::string os_release;
        std::string build_date;
        std::string copyright;
    };
}