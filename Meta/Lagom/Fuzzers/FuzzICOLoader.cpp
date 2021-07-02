// includes
#include <LibGfx/ICOLoader.h>
#include <stddef.h>
#include <stdint.h>

// main extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    Gfx::load_ico_from_memory(data, size);
    return 0;
}