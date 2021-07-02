set(SOURCES
    main.cpp
)

pranaos_app(Network.Applet ICON network)
target_link_libraries(Network.Applet LibGUI LibCore LibGfx)
