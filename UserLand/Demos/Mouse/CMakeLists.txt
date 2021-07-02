set(SOURCES
    main.cpp
)

pranaos_app(Mouse ICON app-mouse)
target_link_libraries(Mouse LibGUI LibGfx)
