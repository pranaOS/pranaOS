set(SOURCES
    main.cpp
)

add_executable(statemachinegenerator ${SOURCES})
target_link_libraries(statemachinegenerator LagomCore)
