OUTPUT_FORMAT("elf64-littleaarch64")

ENTRY(init)

PHDRS
{
        text PT_LOAD FLAGS(5);          
        rodata PT_LOAD FLAGS(4);        
        data PT_LOAD FLAGS(6);          
}

SECTIONS
{
        . = SIZEOF_HEADERS;
        .text : { *(.text) *(.text.*) } :text
        . = ALIGN(4096);
        .rodata : { *(.rodata) *(.rodata.*) } :rodata

        . = ALIGN(4096);
        .data : { *(.data) *(.data.*) } :data
        .bss : { *(.bss) *(.bss.*) } :data

        /DISCARD/ : { *(.interp) }
}