#include "datetime.h"
#include "syscall.h"

using namespace Datetime;

datetime dateTime::current()
{
    datetime result;
    DoSyscall(SYSCALL_GET_DATETIME, (uint32_t)&result);
    return result;
}

char* dateTime::toString() {
}