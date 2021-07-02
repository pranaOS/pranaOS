set(SOURCES
    main.cpp
)

pranaos_app(LibGfxDemo ICON app-libgfx-demo)
target_link_libraries(LibGfxDemo LibGUI LibIPC LibGfx LibCore)
