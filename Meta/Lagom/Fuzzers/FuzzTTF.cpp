// includes
#include <LibTTF/Font.h>
#include <stddef.h>
#include <stdint.h>

// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t data, size_t size)
{
    ByteBuffer font_data = ByteBuffer::copy(data, size);
    TTF::Font::load_from_memory(font_data);
    return 0;
}