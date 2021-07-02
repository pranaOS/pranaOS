// includes
#include <AK/StringView.h>
#include <AK/URL.h>
#include <LibGemini/Document.h>
#include <stddef.h>
#include <stdint.h>

// extern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto gemini = StringView(static_cast<const unsigned char*>(data), size);
    Gemini::Document::parse(gemini, {});
    return 0;
}
