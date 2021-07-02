set(SOURCES
        Tar.cpp
        TarStream.cpp
        Zip.cpp
        )

pranaos_lib(LibArchive archive)
target_link_libraries(LibArchive LibCore)
