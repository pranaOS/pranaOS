#include <libpranaos/utils/bitmap.h>

namespace utils {
    bool bitmap::get(int index) {
        uint8_t bit = index % 8;
        uint16_t byte = index / 16;

        return _buff[byte] & (1 << bit);
    }


}