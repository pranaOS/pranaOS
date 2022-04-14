#pragma once

#include <ak/types.h>
#include <ak/memoperator.h>
#include "stream.h"

namespace Kernel {

        class FIFOStream : public Stream {
        public:
            FIFOStream(int capacity = 100);
            ~FIFOStream();

            char read();
            void write(char byte);
            int available();

        private:
            char* buffer;
            char* buffer_end;
            int count;
            int capacity;
            char* head;
            char* tail;
        };
}