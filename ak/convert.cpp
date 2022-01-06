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

}