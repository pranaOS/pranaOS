set(SOURCES
    main.cpp
)

pranaos_app(KeyboardSettings ICON app-keyboard-settings)
target_link_libraries(KeyboardSettings LibGUI LibKeyboard)
