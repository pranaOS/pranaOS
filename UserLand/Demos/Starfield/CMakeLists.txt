set(SOURCES
    Starfield.cpp
)

pranaos_app(Starfield ICON app-screensaver)
target_link_libraries(Starfield LibGUI LibCore LibGfx)
