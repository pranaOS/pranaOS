set(SOURCES
    Screensaver.cpp
)

pranaos_app(Screensaver ICON app-screensaver)
target_link_libraries(Screensaver LibGUI LibCore LibGfx)
