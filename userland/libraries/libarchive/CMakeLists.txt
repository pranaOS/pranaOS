set(SOURCES
        tar.cpp
        tarstream.cpp
        zip.cpp
        )

pranaos_lib(libarchive archive)
target_link_libraries(libarchive libcore)
