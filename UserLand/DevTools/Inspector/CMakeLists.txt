set(SOURCES
    main.cpp
    RemoteObject.cpp
    RemoteObjectGraphModel.cpp
    RemoteObjectPropertyModel.cpp
    RemoteProcess.cpp
)

pranaos_app(Inspector ICON app-inspector)
target_link_libraries(Inspector LibDesktop LibGUI)
