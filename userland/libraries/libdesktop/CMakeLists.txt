set(SOURCES
    appfile.cpp
    launcher.cpp
)

set(GENERATED_SOURCES
    ../../services/launchserver/launchclientendpoint.h
    ../../services/launchserver/launchserverendpoint.h
)

pranaos_lib(libdesktop desktop)
target_link_libraries(libdesktop libcore libipc libgui)
