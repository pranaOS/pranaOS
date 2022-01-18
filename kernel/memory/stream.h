#pragma once

#include <ak/types.h>

namespace Kernel {
    class Stream {
    public:
        Stream();
        virtual ~Stream();

        virtual char read();
        virtual void write(char byte);
        virtual int available();
    };
}