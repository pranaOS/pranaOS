#pragma once

#include <cstdint>
#include <string>
#include <list>
#include <utility>

namespace utils {
    class formatter {
    private:
        struct argument {
        private:
            uintptr_t _type = 0;
            void* _data = nullptr;

            void* (*_recreate)(void* other) = nullptr;

            template <typename T>
            static uintptr_t get_typeid()
            {
                static int unused;
                return reinterpret_cast<uintptr_t>(&unused);
            }
        
        public:
            template <typename T>
            argument(const T& data)
            {
                set(data);
            }
        };
    };
};
