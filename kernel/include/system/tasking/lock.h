#pragma once

#include <ak/types.h>

/**
 * @brief mutex lock
 */

namespace pranaOS {
    namespace system {
        class MutexLock {

        private:
            int value = 0;

        public:
            MutexLock();

            void lock();
            void unlock();
            void load();
        }
    }
}