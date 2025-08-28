// #include <libc/datetime.h>
// #include <libc/syscall.h>

#include <datetime.h>
#include <syscall.h>
#include <iostream>
#include <cassert>

using namespace Core;
using namespace LibC;

void test_create_and_accessors()
{
    auto dt = DateTime::create(2025, 9, 1, 12, 30, 45);

    assert(dt.year() == 2025);
    assert(dt.month() == 9);
    assert(dt.day() == 1);
    assert(dt.hour() == 12);
    assert(dt.minute() == 30);
    assert(dt.second() == 45);

    std::cout << "[PASS] test_create_and_accessors\n";
}

Time dateTime::current() {
    Time result;
    doSyscall(SYSCALL_GET_DATETIME, (uint32_t)&result);
    return result;
}

Time dateTime::year() {
    Time result;
    doSyscall(SYSCALL_GET_DATETIME, (uint32_t)&result);
    return result;
}