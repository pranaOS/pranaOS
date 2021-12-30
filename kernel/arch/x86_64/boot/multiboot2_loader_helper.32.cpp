#include <stdint.h>
#include <stddef.h>

#define TEXT32 __attribute__((section(".text32")))
#define COM1 0x3f8

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

static int vga_row = 0, vga_column = 0;

static uint8_t inb(uint16_t port)
{
    unsigned char rv;
    __asm__ __volatile__("inb %1, %0"
                         : "=a"(rv)
                         : "dN"(port));
    return rv;
}

static void outb(uint16_t port, uint8_t data)
{
    __asm__ __volatile__("outb %1, %0"
                         :
                         : "dN"(port), "a"(data));
}

static void outw(uint16_t port, uint16_t data)
{
    asm volatile("outw %%ax, %%dx"
                 :
                 : "d"(port), "a"(data));
}

static void initialize_com_port(int addr, int baud_rate)
{
    outb(addr + 1, 0x00); 

    outb(addr + 3, 0x80); 
    outb(addr + 0, 115200 / baud_rate); 
    outb(addr + 1, 0x00);               
    outb(addr + 3, 0x03); 

    outb(addr + 2, 0x00); 
    outb(addr + 4, 0x01); 
}

static bool is_transmit_empty()
{
    return inb(COM1 + 5) & 0x20;
}

static size_t strlen(const char* str) 
{
    size_t len = 0;

    while(str[len])
    {
        len++;
    }

    return len;
}

static void vga_text_console_write_char(char data)
{
    uint16_t* vidmem = (uint16_t*)VGA_ADDRESS;

    const size_t index = vga_row * VGA_WIDTH + vga_column;
    vidmem[index] = (uint16_t)data | (uint16_t)15 << 8;

    if(++vga_column == VGA_WIDTH)
    {
        vga_column = 0;
        if(++vga_row == VGA_HEIGHT)
        {
            vga_row = 0;
        }
    }
}

static void vga_text_console_write(const char* data)
{
    for(int i = 0; i < strlen(data); i++)
    {
        char c = data[i];

        if(c != '\n')
        {
            vga_text_console_write_char(c);
        }
    }
}

extern "C"
{
void TEXT32 multiboot2_helper_com_write_char(char c)
{
    while(is_transmit_empty() == 0);

    outb(COM1, c);
}

void TEXT32 multiboot2_helper_com_write(const char* s)
{
    for(int i = 0; i < strlen(s); i++)
    {
        multiboot2_helper_com_write_char(s[i]);
    }
}

void TEXT32 multiboot2_helper_vga_write_char(char c)
{
    vga_text_console_write_char(c);
}

void TEXT32 multiboot2_helper_vga_write(const char* s)
{
    vga_text_console_write(s);
}

void TEXT32 multiboot2_helper_init()
{
    initialize_com_port(COM1, 9600);
}
}