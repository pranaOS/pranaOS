#include "convert.h"
#include "string.h"

using namespace pranaOS::ak;

int isspace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}