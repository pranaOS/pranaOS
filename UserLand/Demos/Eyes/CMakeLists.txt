set(SOURCES
    main.cpp
    EyesWidget.cpp
)

pranaos_app(Eyes ICON app-eyes)
target_link_libraries(Eyes LibGUI LibGfx)
