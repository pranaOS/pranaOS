set(SOURCES
    PreviewWidget.cpp
    main.cpp
)

pranaos_app(ThemeEditor ICON app-theme-editor)
target_link_libraries(ThemeEditor LibGUI)
