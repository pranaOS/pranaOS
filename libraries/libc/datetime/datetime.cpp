#include "datetime.h"
#include "../syscall.h"
#include "../types.h"
#include "../convert.h"
#include "string.h"
#include "../log.h"

using namespace LibC;

datetime dateTime::current()
{
    datetime result;
    DoSyscall(SYSCALL_GET_DATETIME, (uint32_t)&result);
    return result;
}

char* dateTime::toString() {
    static char resultBuffer[60];
    memset(resultBuffer, 0, 60);

    char* resultPointer = resultBuffer;
    memcpy(resultPointer);

    return resultBuffer;
}