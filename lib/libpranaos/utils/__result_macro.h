#define RESULT_MACRO(X)                             \
    X(SUCCESS, "success")                            \
    X(ERR_UNIMPLEMENTED, "unimplemented function")    \
    X(ERR_OUT_OF_BOUNDS, "out of bounds")              \
    X(ERR_OUT_OF_MEMORY, "out of memory")               \
    X(ERR_INVALID_ADDRESS, "invalid address")            \
    X(ERR_INVALID_ARGUMENT, "invalid argument")           \
    X(ERR_INVALID_COMMAND, "invalid command")              \
    X(ERR_INVALID_FD, "invalid file descriptor")            \
    X(ERR_INVALID_PID, "invalid process id")                 \
    X(ERR_INVALID_SYSCALL, "invalid system call")             \
    X(ERR_INVALID_SIGNAL, "invalid signal")                    \
    X(ERR_ACCESS_DENIED, "access denied")                       \
    X(ERR_NOT_READABLE, "not readable")                          \
    X(ERR_NOT_WRITABLE, "not writable")                           \
    X(ERR_READ_ONLY_FS, "read-only filesystem")                    \
    X(ERR_FILE_IS_A_DIRECTORY, "file is a directory")               \
    X(ERR_FILE_EXISTS, "file already exists")                        \
    X(ERR_NOT_A_DIRECTORY, "not a directory")                         \
    X(ERR_IS_TERMINAL, "refers to a terminal device")                  \
    X(ERR_UNKNOWN, "unknown error")