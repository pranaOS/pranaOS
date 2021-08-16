include(unicode_data.cmake)

SET(SOURCES
    ${UNICODE_DATA_SOURCES}
    CharacterTypes.cpp
)

pranaos_lib(libunicode unicode)
target_link_libraries(libunicode libcore)
