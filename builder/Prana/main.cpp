#include "Config.h"
#include "Context.h"
#include "Executor/Executor.h"
#include "Finder/Finder.h"
#include "Utils/Logger.h"

#include <thread>

int main(int argc, char** argv)
{
    Config::the().process_arguments(argc, argv);

    auto prana_files = Finder::FindRootPranaFiles();

    if (prana_files.size() > 1) {
        Log(Color::Red, "multiple root files are presented");
        exit(1);
    }

    if (prana_files.empty()) {
        Log(Color::Red, "no root files are presented");
        exit(1);
    }

    auto context = Context(prana_files.front(), Context::Operation::Build, {}, true);
    context.run();

    Executor::the().run();

    while (!context.done()) {
        std::this_thread::yield();
    }

    return 0;
}