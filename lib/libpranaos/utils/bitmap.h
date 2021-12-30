#pragma once

#include <cstdint>
#include <cstddef>

namespace utils {
    class bitmap {
    private:
        uint8_t* _buff = nullptr;
        size_t _size = 0;

    public:
        bitmap(uint8_t* buff, size_t size) :
            _buff(buff),
            _size(size)
        {
        }

        void reassign(uint8_t* buff, size_t size)
        {
            _buff = buff;
            _size = size
        }
    };
}