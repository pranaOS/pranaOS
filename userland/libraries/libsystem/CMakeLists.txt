set(SOURCES
    syscall.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib")
pranaos_libc(libsystem system)
target_include_directories(libsystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(libsystemstatic STATIC ${SOURCES})
target_include_directories(libsystemstatic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
