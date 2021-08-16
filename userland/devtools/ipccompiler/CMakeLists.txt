set(SOURCES
    main.cpp
)

add_executable(ipccompiler ${SOURCES})
target_link_libraries(ipccompiler LagomCore)
