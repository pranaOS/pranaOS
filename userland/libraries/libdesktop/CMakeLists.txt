set(SOURCES
    AppFile.cpp
    Launcher.cpp
)

set(GENERATED_SOURCES
    ../../services/LaunchServer/LaunchClientEndpoint.h
    ../../services/LaunchServer/LaunchServerEndpoint.h
)

serenity_lib(libdesktop desktop)
target_link_libraries(libdesktop libcore libipc libgui)
