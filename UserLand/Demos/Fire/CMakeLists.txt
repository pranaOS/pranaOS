set(SOURCES
    Fire.cpp
)

pranaos_app(Fire ICON app-fire)
target_link_libraries(Fire LibGUI LibCore LibGfx)
