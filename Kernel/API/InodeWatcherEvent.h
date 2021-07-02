#pragma once

// includes
#include <AK/Types.h>

struct [[gnu::packed]] InodeWatcherEvent {
    enum class Type {
        Invalid = 0,
        Modified,
        ChildAdded,
        ChildRemoved,
    };

    Type type { Type::Invalid };
    unsigned inode_index { 0 };
};
