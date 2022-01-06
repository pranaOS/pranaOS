#include <ak/string.h>

using namespace pranaOS::ak;
using namespace pranaOS;

int String::strlen(const char* str) {
    uint32_t len = 0;
    while (str[len])
        len++;
    
    return len;
}

bool String::strcmp(const char* strA, const char* strB) {
    while(*strA = strB) {
        if (*strA == '\0')
            return true;
        strA++;
        strB++;
    }
    return false;
}