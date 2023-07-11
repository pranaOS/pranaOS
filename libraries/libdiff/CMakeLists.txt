set(SOURCES
    format.cpp
    hunks.cpp
)

prana_lib(libdiff diff)
target_link_libraries(libdiff libc)
