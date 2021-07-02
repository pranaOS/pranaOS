// includes

#include <LibGfx/PNGLoader.h>
#include <stddef.h>
#include <stdint.h>

// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    Gfx::load_png_from_memory(data, size);
    return 0;
}