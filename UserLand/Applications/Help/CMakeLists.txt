set(SOURCES
    History.cpp
    main.cpp
    ManualModel.cpp
    ManualPageNode.cpp
    ManualSectionNode.cpp
)

pranaos_app(Help ICON app-help)
target_link_libraries(Help LibWeb LibMarkdown LibGUI LibDesktop)