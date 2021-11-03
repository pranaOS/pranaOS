OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start32)

SECTIONS
{
    . = 1M;

    PROVIDE(kernel_start = .);
	PROVIDE(kernel_text_start = .);
	.text : ALIGN(4K)
	{
		*(.multiboot_header)
		*(.text)
	}
    PROVIDE(kernel_text_end = .);

	PROVIDE(kernel_rodata_start = .);
	.rodata : ALIGN(4K)
	{
		*(.rodata)
	}
	PROVIDE(kernel_rodata_end = .);

    PROVIDE(kernel_data_start = .);
    .data : ALIGN(4K)
    {
		PROVIDE(ctors_start = .);
	    *(.ctor*)
	    PROVIDE(ctors_end = .);
	    PROVIDE(dtors_start = .);
	    *(.dtor*)
	    PROVIDE(dtors_end = .);
        *(.data)
    }
    PROVIDE(kernel_data_end = .);

	PROVIDE(kernel_bss_start = .);
	.bss : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
	PROVIDE(kernel_bss_end = .);

    PROVIDE(kernel_end = .);
}