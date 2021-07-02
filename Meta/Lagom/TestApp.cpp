#include <LibCore/EventLoop.h>
#include <LibCore/Timer.h>
#include <stdio.h>

// main function
int main(int, char**)
{
    Core::EventLoop event_loop;

    auto timer = Core::Timer::construct(100, [&] {
        dbgln("Timer fired, good-bye! :^)");
        event_loop.quit(0);
    });

    return event_loop.exec();
}