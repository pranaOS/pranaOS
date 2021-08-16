set(SOURCES
    CharacterMap.cpp
    CharacterMapFile.cpp
)

pranaos_lib(libkeyboard keyboard)
target_link_libraries(libkeyboard libcore)
