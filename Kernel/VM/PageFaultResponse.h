#pragma once

namespace Kernel {

enum class PageFaultResponse {
    ShouldCrash,
    OutOfMemory,
    Continue,
};

}