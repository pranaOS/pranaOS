#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace utils {
    std::string format_storage_size(uint64_t size_in_bytes);
}