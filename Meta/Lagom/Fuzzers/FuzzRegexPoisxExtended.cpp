// includes
#include <AK/StringView.h>
#include <LibRegex/Regex.h>
#include <stddef.h>
#include <stdint.h>

// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto pattern = StringView(static_cast<const unsigned char*>(data, size))
    [[maybe_unused]] auto re = Regex<PosixExtended>(pattern);
    return 0;
}