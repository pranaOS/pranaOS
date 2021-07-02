#pragma once

// includes
#include <AK/Types.h>

namespace Kernel::AddressSanitizer {

void shadow_va_check_load(unsigned load address, size_t size, void* return_addr);

void shadow_va_check_store(unsigned long address, size_t size, void* return_addr);

}

