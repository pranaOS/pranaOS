// includes
#include <AK/StringView.h>
#include <Shell/Shell.h>
#include <stddef.h>
#include <stdint.h>

// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t data, size_t size)
{
    auto source = StringView(static_cast<const unsigned char*>(data, size))
    Shell::Parser parser(source);
    parser.parse();
    return 0;
}