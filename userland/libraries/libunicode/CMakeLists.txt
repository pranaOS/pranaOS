include(${pranaOS_SOURCE_DIR}/meta/CMake/unicode_data.cmake)

if (DEFINED UNICODE_DATA_SOURCES)
    set(SOURCES ${UNICODE_DATA_SOURCES})
    pranaos_lib(libunicodedata unicodedata)
    target_compile_options(libunicodedata PRIVATE -g0 -Os)
    target_link_libraries(libunicodedata libcore libtimezone)
endif()

set(SOURCES
    charactertypes.cpp
    currencycode.cpp
    datetimeformat.cpp
    locale.cpp
    numberformat.cpp
    relativeTimeformat.cpp
)

pranaos_lib(libunicode unicode)
target_link_libraries(libunicode libcore)
target_compile_definitions(libunicode PRIVATE ENABLE_UNICODE_DATA=$<BOOL:${ENABLE_UNICODE_DATABASE_DOWNLOAD}>)

if (DEFINED UNICODE_DATA_SOURCES)
    add_dependencies(libunicode libunicodedata)
endif()
