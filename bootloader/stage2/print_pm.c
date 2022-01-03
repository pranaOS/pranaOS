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


void print(char *txt)
{
	char c;
	size_t i = 0;


	while ((c = txt[i]) != 0)
	{
		if (c == '\n')
		{
			loc += (VIDEO_COLS - ((loc/2) % VIDEO_COLS)) * 2;
		}
		else 
		{
			print_char(txt[i]);
		}
		i++;
	}

}

int strlen(char *str)
{
    int size = 0;
    while (str[size])
        size++;
    
    return size;
}