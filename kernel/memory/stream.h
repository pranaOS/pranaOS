#pragma once

#include <ak/types.h>

namespace Kernel {
    namespace system {
        class Stream {
        public:
            Stream();

            virtual ~Stream();

            virtual char read();
            virtual char overwrite();
            virtual void write(char byte);
            virtual char available();
        };
    }
}