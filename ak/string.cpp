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
    while(*strA == *strB)
    {
        if(*strA == '\0')
            return true; 
        ++strA;
        ++strB;
    }
    return false;
}

bool String::strncmp(const char* s1, const char* s2, int n) {
    while(n--)
        if(*s1++ != *s2++)
            return false;

    return true; 
}

int String::indexof(const char* str, char c, uint32_t skip) {
    uint32_t hits = 0;
    int i = 0;
    while(str[i]) {
        if (str[i] == c && hits++ == skip)
            return i;
        i++;
    }

    return -1;
}

bool String::contains(const char* str, char c) {
    int i = 0;
    while (str[i]) {
        if (str[i] == c)
            return true;
            i++;
    }

    return false;
}

List<char*> String::Split(const char* str, char d)
{
    List<char*> result = List<char*>();
    int len = String::strlen(str);
    int pos = 0;

    for(int i = 0; i < len; i++) {
        if(str[i] == d) {
            int itemLen = i - pos;
            if(itemLen > 0) {
                char* part = new char[itemLen + 1];
                MemOperator::memcpy(part, str + pos, itemLen);
                part[itemLen] = '\0';
                result.push_back(part);
            }

            pos = i + 1;   
        }
    }

    if(pos > 0) {
        int lastLen = len - pos;
        if(lastLen > 0) {
            char* part = new char[lastLen + 1];
            MemoryOperations::memcpy(part, str + pos, lastLen);
            part[lastLen] = '\0';
            result.push_back(part);
        }
    }
    return result;
}