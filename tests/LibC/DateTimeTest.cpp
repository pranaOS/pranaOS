// #include <libc/datetime.h>
// #include <libc/syscall.h>

#include <datetime.h>
#include <syscall.h>

using namespace LibC;

Time dateTime::current() {
    Time result;
    doSyscall(SYSCALL_GET_DATETIME, (uint32_t)&result);
    return result;
}