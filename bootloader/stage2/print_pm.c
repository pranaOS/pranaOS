#include <types.h>

#define VIDEO_MEM 0xb8000
#define VIDEO_COLS 80
#define VIDEO_ROWS 25 

size_t loc = 0;

void print_char(char c)
{
    char *videoMem = (char*) VIDEO_MEM;
    videoMem[loc] = c;
    videoMem[loc+1] = 0x0F;
    loc += 2;
}