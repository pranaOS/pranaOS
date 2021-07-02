// includes
#include <LibAudio/WavLoader.h>
#include <stddef.h>
#include <stdint.h>

// exetern c function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto wav_data = ByteBuffer::copy(data, size);
    auto wav = make<Audio::WavLoaderPlugin>(wav_data);

    if (!wav->sniff())
        return 1;

    while (wav->get_more_samples())
        ;

    return 0;
}