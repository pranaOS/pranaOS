set(SOURCES
    main.cpp
    QSWidget.cpp
)

pranaos_app(QuickShow ICON filetype-image)
target_link_libraries(QuickShow LibDesktop LibGUI LibGfx)
