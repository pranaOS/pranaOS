set(SOURCES
    Account.cpp
    AnonymousBuffer.cpp
    ArgsParser.cpp
    ConfigFile.cpp
    Command.cpp
    DateTime.cpp
    DirIterator.cpp
    ElapsedTimer.cpp
    Event.cpp
    EventLoop.cpp
    FileWatcher.cpp
    File.cpp
    GetPassword.cpp
    IODevice.cpp
    LocalServer.cpp
    LocalSocket.cpp
    LockFile.cpp
    MimeData.cpp
    NetworkJob.cpp
    NetworkResponse.cpp
    Notifier.cpp
    Object.cpp
    Process.cpp
    ProcessStatisticsReader.cpp
    Property.cpp
    Socket.cpp
    StandardPaths.cpp
    TCPServer.cpp
    TCPSocket.cpp
    Timer.cpp
    UDPServer.cpp
    UDPSocket.cpp
)

pranaos_lib(libcore core)
target_link_libraries(libcore libc libcrypt)
