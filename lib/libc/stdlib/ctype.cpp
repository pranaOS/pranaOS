#include <cctype>

int isalpha(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isspace(int c)
{
    return c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r';
}

int isdigit(int c)
{
    return c >= '0' && c <= '9';
}

int isupper(int c)
{
    return c >= 'A' && c <= 'Z';
}

int islower(int c)
{
    return c >= 'a' && c <= 'z';
}

int isprint(int c)
{
    return c >= 0x20 && c <= 0x7e;
}

int isalnum(int c)
{
    return isalpha(c) || isdigit(c);
}

int iscntrl(int c)
{
    return c == 0x7f || (c >= 0 && c <= 0x1f);
}

int isxdigit(int c)
{
    return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

int tolower(int c)
{
    return isupper(c) ? c - 'A' + 'a' : c;
}

int toupper(int c)
{
    return islower(c) ? c - 'a' + 'A' : c;
}