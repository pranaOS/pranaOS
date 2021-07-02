set(SOURCES
    KeyboardMapperWidget.cpp
    KeyButton.cpp
    main.cpp
)

pranaos_bin(KeyboardMapper)
target_link_libraries(KeyboardMapper LibGUI LibKeyboard)
