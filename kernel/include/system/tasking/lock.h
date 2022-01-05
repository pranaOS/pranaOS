#pragma once

#include <ak/types.h>

/**
 * @brief mutex lock
 */

namespace pranaOS {
    namespace system {
        class mutexLock {

        private:
            int value = 0;

        public:
            mutexLock();

            void lock();
            void unlock();
            void load();
        }
    }
}