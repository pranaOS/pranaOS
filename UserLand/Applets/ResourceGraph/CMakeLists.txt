set(SOURCES
    main.cpp
)

pranaos_bin(ResourceGraph.Applet)
target_link_libraries(ResourceGraph.Applet LibGUI LibCore LibGfx)
