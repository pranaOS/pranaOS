// includes
#include <LibCompress/Zlib.h>
#include <stdio.h>

// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto result = Compress::Zlib::decompress_all(ReadonlyBytes { data, size });
    return result.has_value();
}
