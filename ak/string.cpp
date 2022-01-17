#include "string.h"

using namespace pranaOS;
using namespace pranaOS::ak;

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
    while(str[i])
    {
        if(str[i] == c && hits++ == skip)
            return i;
        i++;
    }
    return -1;
}

bool String::contains(const char* str, char c) {
    int i = 0;
	while (str[i])
    {
        if(str[i] == c)
            return true;
		i++;
    }
    return false;
}

List<char*> String::split(const char* str, char d) {
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
            MemOperator::memcpy(part, str + pos, lastLen);
            part[lastLen] = '\0';
            result.push_back(part);
        }
    }
    return result;
}

char* String::uppercase(char* str) { 
    int len = strlen(str);
    int i = 0;
	while (i < len)
	{
		if ((short)str[i] >= 97 && (short)str[i] <= 122)
			str[i] -= 32;
		i++;
	}
    return str;
}

char* String::lowercase(char* str) {
    int len = strlen(str);
    int i = 0;
	while (i < len)
	{
		if ((short)str[i] >= 65 && (short)str[i] <= 90)
			str[i] += 32;
		i++;
	}
    return str;
}

char String::uppercase(char c) {
    if (c >= 97 && c <= 122)
		return c - 32;
    
    return c;
}

char String::lowercase(char c) {
	if (c >= 65 && c <= 90)
		return c + 32;
    
    return c;
}

char* String::strcpy(char *s1, const char *s2) {
	strncpy(s1, s2, strlen(s2) + 1);
	s1[strlen(s2)] = '\0'; 
	return s1;
}

char* String::strncpy(char *s1, const char *s2, unsigned int n) {
	unsigned int extern_iter = 0;

	unsigned int iterator = 0;
	for (iterator = 0; iterator < n; iterator++) 
	{
		if (s2[iterator] != '\0')
			s1[iterator] = s2[iterator];
		else 
		{
			s1[iterator] = s2[iterator];
			extern_iter = iterator + 1;
			break;
		}
	}

	while (extern_iter < n) 
	{
		s1[extern_iter] = '\0';
		extern_iter++;
	}

	return s1;
}