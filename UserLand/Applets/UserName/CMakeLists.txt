set(SOURCES
    main.cpp
)

pranaos_bin(UserName.Applet)
target_link_libraries(UserName.Applet LibGUI LibGfx)
