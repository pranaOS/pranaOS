OUTPUT_FORMAT("elf64-littleaarch64")

SECTIONS
{
    . = SIZEOF_HEADERS;
    .hash : { *(.hash) } : text
    .gnu.hash : { *(.gnu.hash) } : text
    .dynsym : { *(.dynsym) } : text
    .dynstr : { *(.dynstr) } : text
    .dynamic : { *(.dynamic) } : dynamic : text

    .gnu.version : { *(.gnu.version) } : text
    .gnu.version_d : { *(.gnu.version_d) } : text
    .gnu.version_r : { *(.gnu.version_r) } : text

    .plt : { *(.plt) } : text
    .got : { *(.got) } : text
    .got.plt : { *(.got.plt) } : text

    .eh_frame : { *(.eh_frame) } : text
    .text : { *(.text*) } : text

    vvar_page = ALIGN(4096);
    __vdso_vdso_dat = vvar_page + 128;
    pvclock_page = vvar_page + 4096;
}

PHDRS
{
    text PT_LOAD FILEHDR PHDRS FLAGS(5); 
    dynamic PT_DYNAMIC FLAGS(4); 
}

VERSION
{
    LINUX_2.6 {
        global:
            rt_sigreturn;
            __vdso_rt_sigreturn;
            gettimeofday;
            __vdso_gettimeofday;
            clock_gettime;
            __vdso_clock_gettime;
            getcpu;
            __vdso_getcpu;
            time;
            __vdso_time;
        local:
            *;
    };
}

VDSO_sigtramp = __vdso_rt_sigreturn;