set(SOURCES
    Decoder.cpp
    Encoder.cpp
    Message.cpp
    Stub.cpp
)

pranaos_lib(libipc ipc)
target_link_libraries(libipc libc libcore)
