// includes
#include <LibGfx/GIFLoader.h>
#include <stddef.h>
#include <stdint.h>

// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    Gfx::load_gif_from_memory(data, size);
    return 0;
}
