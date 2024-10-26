set(SOURCES
    forward.cpp
    pthread.cpp
    pthread_cond.cpp
    pthread_once.cpp
    semaphore.cpp
)

pranaos_libc(libpthread pthread)
target_link_libraries(libpthread libc libsystem)
target_include_directories(libpthread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
