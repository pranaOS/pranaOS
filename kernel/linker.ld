OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

ENTRY(_entrypoint)

SECTIONS
{
  . = 0xC0100000;

  _kernel_base = .;

	.text ALIGN(4K) : AT(ADDR(.text)-0xC0000000)
	{
		*(.multiboot)
		*(.text*)
  }

  .rodata ALIGN (4K) : AT(ADDR(.rodata)-0xC0000000)
	{
		*(.rodata)
  }

  .bss ALIGN(4K) : AT(ADDR(.bss)-0xC0000000)
  {
    *(.COMMON)
    *(.bss)
    *(.bootstrap_stack)
  }

  .data ALIGN(4K) : AT(ADDR(.data)-0xC0000000)
  {
    start_ctors = .;
    KEEP(*( .init_array ));
    KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
    end_ctors = .;

    *(.data*)
  }

  /DISCARD/ : { *(.fini_array*) *(.comment) }

  _kernel_end = .;
}