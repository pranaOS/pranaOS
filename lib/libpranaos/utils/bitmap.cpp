#include <libpranaos/utils/bitmap.h>

namespace utils {
    bool bitmap::get(int index)
    {
        uint8_t bit = index % 8;
        uint16_t byte = index / 8;

        return _buff[byte] & (1 << bit);
    }

    void bitmap::set(int index, bool value)
    {
        uint8_t bit = index % 8;
        uint16_t byte = index / 8;

        if(value)
        {
            _buff[byte] |= 1 << bit;
        }
        else
        {
            _buff[byte] &= ~(1 << bit);
        }
    }
}