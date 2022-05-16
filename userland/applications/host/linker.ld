ENTRY(_start)

SECTIONS
{
	. = 0x00100000;
 
	.text ALIGN(4096) : AT(ADDR(.text))
	{
		*(.text .text.*)
	}
 
	.rodata ALIGN(4096) : AT(ADDR(.rodata))
	{
		*(.rodata .rodata.*)
	}
 
	.data ALIGN(4096) : AT(ADDR(.data))
	{
		*(.data .data.*)
		*(.symbols)
	}
 
	.bss ALIGN(4096) : AT(ADDR(.bss))
	{
		*(COMMON)
		*(.bss .bss.*)
		*(.stack)
	}
 
 	.eh_frame ALIGN(4096) : AT(ADDR(.eh_frame))
	{
		*(.eh_frame)
	}

	/DISCARD/ :
	{
		*(.comment)
	}
}
