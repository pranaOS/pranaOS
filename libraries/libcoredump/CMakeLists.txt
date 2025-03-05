set(SOURCES
    backtrace.cpp
    inspector.cpp
    reader.cpp
)

pranaos_lib(libcoredump coredump)
target_link_libraries(libcoredump libc libcompress libcore libdebug)
