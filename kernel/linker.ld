ENTRY(_start)

SECTIONS
{
	kernel_boot = 0x00100000;
	kernel_higher_half = 0xc0000000;
	
	. = 0xC0100000;
	kernel_start = .;
 
	.text ALIGN(4096) : AT(ADDR(.text) - kernel_higher_half)
	{
		kernel_text_start = .;
		*(.multiboot)
		*(.text .text.*)
		kernel_text_end = .;
	}
 
	.rodata ALIGN(4096) : AT(ADDR(.rodata) - kernel_higher_half)
	{
		*(.rodata .rodata.*)
	}
 

	.data ALIGN(4096) : AT(ADDR(.data) - kernel_higher_half)
	{
		kernel_data_start = .;
		*(.data .data.*)
		*(.symbols)
		kernel_data_end = .;
	}
 
	.bss ALIGN(4096) : AT(ADDR(.bss) - kernel_higher_half)
	{
		*(COMMON)
		*(.bss .bss.*)
		*(.stack)
	}
 
 	.eh_frame ALIGN(4096) : AT(ADDR(.eh_frame) - kernel_higher_half)
	{
		*(.eh_frame)
	}

	kernel_end = .;

	/DISCARD/ :
	{
		*(.comment)
	}
}