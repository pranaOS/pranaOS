#pragma once

namespace Kernel {
class NetworkTask {
public:
    static void spawn();
    static bool is_current();
};

}