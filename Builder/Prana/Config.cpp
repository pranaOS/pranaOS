#include "Config.h"
#include <iostream>
#include <string>

Config::Config()
{
    const auto now = std::chrono::system_clock::now();
    m_timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}