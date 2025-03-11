set(SOURCES
    buffer.cpp
    resampler.cpp
    sampleformats.cpp
    connectionfromclient.cpp
    loader.cpp
    wavloader.cpp
    flacloader.cpp
    wavwriter.cpp
    mp3loader.cpp
    usersamplequeue.cpp
)

set(GENERATED_SOURCES
    ../../services/audioserver/audioclientendpoint.h
    ../../services/audioserver/audioserverendpoint.h
)

pranaos_lib(LibAudio audio)
target_link_libraries(libaudio libcore libipc libthreading)
