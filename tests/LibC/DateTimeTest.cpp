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

void test_to_string()
{
    auto dt = DateTime::create(2025, 1, 15, 8, 5, 9);

    // default format: "%Y-%m-%d %H:%M:%S"
    String s = dt.to_string();
    std::cout << "Formatted string: " << s.characters() << "\n";

    assert(s.contains("2025"));
    assert(s.contains("01"));
    assert(s.contains("15"));

    std::cout << "[PASS] test_to_string\n";
}

void test_comparison()
{
    auto earlier = DateTime::create(2020, 5, 10);
    auto later   = DateTime::create(2021, 5, 10);

    assert(earlier < later);
    assert(!(later < earlier));

    std::cout << "[PASS] test_comparison\n";
}

void test_now_and_timestamp()
{
    auto now = DateTime::now();
    time_t ts = now.timestamp();

    assert(ts > 0);
    std::cout << "Current timestamp: " << ts << "\n";
    std::cout << "[PASS] test_now_and_timestamp\n";
}

void test_parse()
{
    String input("2025-09-01 12:00:00");
    auto parsed = DateTime::parse("%Y-%m-%d %H:%M:%S"sv, input);

    assert(parsed.has_value());
    assert(parsed->year() == 2025);
    assert(parsed->month() == 9);
    assert(parsed->day() == 1);

    std::cout << "[PASS] test_parse\n";
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