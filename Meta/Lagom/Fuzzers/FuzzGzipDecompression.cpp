// includes
#include <LibCompress/Gzip.h>
#include <stdio.h>

// main extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto result = Compress::GzipDecompressor::decompress_all(ReadonlyBytes { data, size });
    return result.has_value();
}
