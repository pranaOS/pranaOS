#include <types.h>

#define VIDEO_MEM 0xb8000

#define VIDEO_COLS	80
#define VIDEO_ROWS 	25

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

void print_int(int integer)
{
	int tmp = integer;
	int rev_tmp = 0;
	unsigned int z_b = 0;
	while (tmp > 0)
	{
		rev_tmp = rev_tmp * 10 + tmp % 10;
		if ((rev_tmp == 0) && ((tmp % 10) == 0))
		{
			z_b++;
		}
		tmp /= 10;
	}

	if (rev_tmp == 0)
	{
		print_char('0');
	}

	while (rev_tmp > 0)
	{
		print_char('0' + (rev_tmp % 10));
		rev_tmp /= 10;
	}

	for (int i = 0; i < z_b; i++)
	{
		print_char('0');
	}
}