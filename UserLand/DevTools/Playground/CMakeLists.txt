set(SOURCES
    main.cpp
    GMLAutocompleteProvider.cpp
)

pranaos_app(Playground ICON app-playground)
target_link_libraries(Playground LibDesktop LibGUI)
