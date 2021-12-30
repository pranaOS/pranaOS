ENTRY(_start)

SECTIONS
{
	. = 1M;
	_kernel_start = .;

	.multiboot2 ALIGN(4K) :
	{
		KEEP(*(.multiboot2))
	}

	.stivale2hdr ALIGN(4K) :
	{
		KEEP(*(.stivale2hdr))
	}

	.text ALIGN(4K) :
	{
		KEEP(*(.text*))
	}

	.text32 ALIGN(4K) :
	{
		KEEP(*(.text32*))
	}

	.rodata ALIGN(4K) :
	{
		KEEP(*(.rodata*))
	}

	.data ALIGN(4K) :
	{
		KEEP(*(.data*))
	}

	.bss ALIGN(4K) :
    {
		KEEP(*(COMMON))
        KEEP(*(.bss*))
    }

	.symtab ALIGN(4K) :
	{
		KEEP(*(.symtab))
	}

	_kernel_end = .;
}