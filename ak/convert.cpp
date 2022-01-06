#include "convert.h"
#include "string.h"

using namespace pranaOS::ak;

int isspace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}


char* Convert::intToString(int n) {
    static char ret[24];
    int numChars = 0;
    bool isNegative = false;
    if (n < 0)
    {
        n = -n;
        isNegative = true;
        numChars++;
    }

    int temp = n;
    do
    {
        numChars++;
        temp /= 10;
    } while (temp);

    ret[numChars] = 0;

    if (isNegative)
        ret[0] = '-';
    int i = numChars - 1;
    do
    {
        ret[i--] = n % 10 + '0';
        n /= 10;
    } while (n);
    return ret;
}

char* Convert::intToString32(uint32_t n) {
    static char ret[32];
    int numChars = 0;
    int temp = n;
    do
    {
        numChars++;
        temp /= 10;
    } while (temp);

    ret[numChars] = 0;
    
    int i = numChars - 1;
    do
    {
        ret[i--] = n % 10 + '0';
        n /= 10;
    } while (n);
    return ret;
}

char* Convert::IntToHexString(uint8_t w) {
    static const char* digits = "0123456789";
    uint32_t hexSize = sizeof(uint8_t) << 1;
    char* rc = new char[hexSize + 1]; 

    MemOperator::memset(rc, 0, hexSize + 1);

    for (uint32_t i=0, j=(hexSize-1)*4 ; i<hexSize; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];

    return rc;
}

char* Convert::intToHexString(uint16_t w) {
    static const char* digits = "0123456789";
    uint32_t hexSize = sizeof(uint16_t)<<1;
    char* rc = new char[hexSize + 1]; 
    MemOperator::memset(rc, 0, hexSize + 1);

    for (uint32_t i=0, j=(hexSize-1)*4 ; i<hexSize; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    return rc;
}

char* Convert::intToHexString(uint32_t w) {
    static const char* digits = "0123456789";
    uint32_t hexSize = sizeof(uint32_t)<<1;
    char* rc = new char[hexSize + 1]; 

    MemOperator::memset(rc, 0, hexSize + 1);

    for (uint32_t i=0, j=(hexSize-1)*4 ; i<hexSize; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
        
    return rc;
}