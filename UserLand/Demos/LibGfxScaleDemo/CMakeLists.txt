set(SOURCES
    main.cpp
)

pranaos_app(LibGfxScaleDemo ICON app-libgfx-demo)
target_link_libraries(LibGfxScaleDemo LibGUI LibIPC LibGfx LibCore)
