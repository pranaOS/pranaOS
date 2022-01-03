ENTRY(_start)
SECTIONS
{
    .text 0x9000 :
    {
        *(.text)
    }

    .data :
    {
        *(.data)
        *(.rodata)
    }

    .bss :
    {
        *(.bss)
    }

	end_bin = .; _end_bin = .; __end_bin = .;
}