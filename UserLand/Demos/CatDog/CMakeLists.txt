set(SOURCES
    main.cpp
)

pranaos_app(CatDog ICON app-catdog)
target_link_libraries(CatDog LibGUI LibGfx)
