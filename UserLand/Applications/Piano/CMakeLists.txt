set(SOURCES
    Track.cpp
    TrackManager.cpp
    KeysWidget.cpp
    KnobsWidget.cpp
    main.cpp
    MainWidget.cpp
    RollWidget.cpp
    SamplerWidget.cpp
    WaveWidget.cpp
)

pranaos_app(Piano ICON app-piano)
target_link_libraries(Piano LibAudio LibGUI)
