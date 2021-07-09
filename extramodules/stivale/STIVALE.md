# stivale boot protocol specification

The stivale boot protocol aims to be a *simple* to implement protocol which
provides the kernel with most of the features one may need in a *modern*
x86_64 context (although 32-bit x86 is also supported).

## General information

In order to have a stivale compliant kernel, one must have a kernel executable
in the `elf64` or `elf32` format and have a `.stivalehdr` section (described below).
Other executable formats are not supported.

stivale will recognise whether the ELF file is 32-bit or 64-bit and put the CPU into
the appropriate mode before loading the kernel.

stivale natively supports (only for 64-bit kernels) and encourages higher half kernels.
The kernel can load itself at `0xffffffff80000000 + phys_base` (as defined in the linker script),
and the bootloader will take care of everything else, no AT linker script directives needed.

`phys_base` represents the actual physical base address of the kernel.
E.g.: having `. = 0xffffffff80000000 + 2M;` will load the kernel at physical address
2MiB, and at virtual address `0xffffffff80200000`.

For relocatable kernels, the bootloader may change the load addresses of the sections
by adding a slide, if the specified location of a segment is not available for use.

If KASLR is enabled, a random slide will be added unconditionally.

If the kernel loads itself in the lower half, the bootloader will not perform the
higher half relocation.

## Kernel entry machine state

### x86_64

`rip` will be the entry point as defined in the ELF file, unless the `entry_point`
field in the stivale header is set to a non-0 value, in which case, it is set to
the value of `entry_point`.

At entry, the bootloader will have setup paging mappings as such:

```
 Base Physical Address -                    Size                    ->  Virtual address
  0x0000000000000000   - 4 GiB plus any additional memory map entry -> 0x0000000000000000
  0x0000000000000000   - 4 GiB plus any additional memory map entry -> 0xffff800000000000 (4-level paging only)
  0x0000000000000000   - 4 GiB plus any additional memory map entry -> 0xff00000000000000 (5-level paging only)
  0x0000000000000000   -                 0x80000000                 -> 0xffffffff80000000
```

All the mappings are supervisor, read, write, execute (-rwx). The elf sections
of the kernel do not change this.

The bootloader page tables are in bootloader-reclaimable memory, their specific layout
is undefined as long as they provide the above memory mappings.

If the kernel is a position independent executable, the bootloader is free to
relocate it as it sees fit, potentially performing KASLR (as specified by the config).

At entry all segment registers are loaded as 64 bit code/data segments. Limits and
bases are ignored since this is 64-bit mode.

The GDT register is loaded to point to a GDT, in bootloader-reserved memory,
with at least the following entries, starting at offset 0:

  - Null descriptor
  - 16-bit code descriptor. Base = `0`, limit = `0xffff`. Readable.
  - 16-bit data descriptor. Base = `0`, limit = `0xffff`. Writable.
  - 32-bit code descriptor. Base = `0`, limit = `0xffffffff`. Readable.
  - 32-bit data descriptor. Base = `0`, limit = `0xffffffff`. Writable.
  - 64-bit code descriptor. Base and limit irrelevant. Readable.
  - 64-bit data descriptor. Base and limit irrelevant. Writable.

The IDT is in an undefined state. Kernel must load its own.

IF flag, VM flag, and direction flag are cleared on entry. Other flags undefined.

PG is enabled (`cr0`), PE is enabled (`cr0`), PAE is enabled (`cr4`),
LME is enabled (`EFER`).
If stivale header flag bit 1 is set, then, if available, 5-level paging is enabled
(LA57 bit in `cr4`).

The A20 gate is opened.

PIC/APIC IRQs are all masked.

`rsp` is set to the requested stack as per stivale header. If the requested value is
non-null, an invalid return address of 0 is pushed to the stack before jumping
to the kernel.

`rdi` will contain the address of the stivale structure (described below).

All other general purpose registers are set to 0.

### IA-32

`eip` will be the entry point as defined in the ELF file, unless the `entry_point`
field in the stivale header is set to a non-0 value, in which case, it is set to
the value of `entry_point`.

At entry all segment registers are loaded as 32 bit code/data segments.
All segment bases are `0x00000000` and all limits are `0xffffffff`.

The GDT register is loaded to point to a GDT, in bootloader-reserved memory,
with at least the following entries, starting at offset 0:

  - Null descriptor
  - 16-bit code descriptor. Base = `0`, limit = `0xffff`. Readable.
  - 16-bit data descriptor. Base = `0`, limit = `0xffff`. Writable.
  - 32-bit code descriptor. Base = `0`, limit = `0xffffffff`. Readable.
  - 32-bit data descriptor. Base = `0`, limit = `0xffffffff`. Writable.
  - 64-bit code descriptor. Base and limit irrelevant. Readable.
  - 64-bit data descriptor. Base and limit irrelevant. Writable.

The IDT is in an undefined state. Kernel must load its own.

IF flag, VM flag, and direction flag are cleared on entry. Other flags undefined.

PE is enabled (`cr0`).

The A20 gate is enabled.

PIC/APIC IRQs are all masked.

`esp` is set to the requested stack as per stivale header. An invalid return address
of 0 is pushed to the stack before jumping to the kernel.

The address of the stivale structure (described below) is pushed onto this stack
before the entry point is called.

All other general purpose registers are set to 0.

## Low memory area

For x86_64 and IA-32, stivale guarantees that an area of no less than 32 KiB is
free and usable at physical memory address `0x70000`, regardless of what is
specified in the memory map.

This allows the kernel to perform tasks that require Real Mode accessible memory
such as multicore startup.

## Bootloader-reserved memory

In order for stivale to function, it needs to reserve memory areas for either internal
usage (such as page tables, GDT), or for kernel interfacing (such as returned
structures).

stivale ensures that none of these areas are found in any of the sections
marked as "usable" or "kernel/modules" in the memory map.

The location of these areas may vary and it is implementation specific, but they
are guaranteed to be in "bootloader reclaimable" entries.

Once the OS is done needing the bootloader, memory map areas marked as "bootloader
reclaimable" may be used as usable memory. These areas are guaranteed to be aligned
to the smallest possible page size (4K on x86_64 and IA-32), for both base and length,
and they are guaranteed to not overlap other sections of the memory map.

## stivale header (.stivalehdr)

The kernel executable shall have a section `.stivalehdr` which will contain
the header that the bootloader will parse.

Said header looks like this:
```c
struct stivale_header {
    uint64_t stack;   // This is the stack address which will be in ESP/RSP
                      // when the kernel is loaded.
                      // It can only be set to NULL for 64-bit kernels. 32-bit
                      // kernels are mandated to provide a vaild stack.
                      // 64-bit and 32-bit valid stacks must be at least 256 bytes
                      // in usable space and must be 16 byte aligned addresses.

    uint16_t flags;   // Flags
                      // bit 0  0 = text mode, 1 = graphics framebuffer mode
                      // bit 1  0 = 4-level paging, 1 = use 5-level paging (if
                      //                                available)
                      //        Ignored if booting a 32-bit kernel.
                      // bit 2  Formerly used to indicate whether to enable KASLR,
                      //        this flag is now reserved as KASLR is enabled in the
                      //        bootloader configuration instead. Presently
                      //        reserved and unused.
                      // bit 3  If set to 1, all pointers, except otherwise noted,
                      //        are to be offset to the higher half. That is,
                      //        their value will be their physical address plus
                      //        0xffff800000000000 with 4-level paging or
                      //        0xff00000000000000 with 5-level paging on x86_64.
                      //        Success for this feature can be tested by checking
                      //        whether the stivale struct pointer argument passed
                      //        to the entry point function is in the higher
                      //        half or not.
                      // All other bits are undefined and must be 0.

    uint16_t framebuffer_width;   // These 3 values are parsed if a graphics mode
    uint16_t framebuffer_height;  // is requested. If all values are set to 0
    uint16_t framebuffer_bpp;     // then the bootloader will pick the best possible
                                  // video mode automatically (recommended).
    uint64_t entry_point;      // If not 0, this field will be jumped to at entry
                               // instead of the ELF entry point.
} __attribute__((packed));
```

## stivale structure

The stivale structure returned by the bootloader looks like this:
```c
struct stivale_struct {
    uint64_t cmdline;               // Address of a null-terminated cmdline
    uint64_t memory_map_addr;       // Address of the memory map (entries described below)
    uint64_t memory_map_entries;    // Count of memory map entries
    uint64_t framebuffer_addr;      // Address of the graphical framebuffer if available.
                                    // Else, 0
    uint16_t framebuffer_pitch;     // Pitch of the framebuffer in bytes
    uint16_t framebuffer_width;     // Width and height of the framebuffer in pixels
    uint16_t framebuffer_height;
    uint16_t framebuffer_bpp;       // Bits per pixel
    uint64_t rsdp;                  // Address the ACPI RSDP structure
    uint64_t module_count;          // Count of modules that stivale loaded according to config
    uint64_t modules;               // Address of the first entry in the linked list of modules (described below)
    uint64_t epoch;                 // UNIX epoch at boot, read from system RTC
    uint64_t flags;                 // Flags
                                    // bit 0: 1 if booted with BIOS, 0 if booted with UEFI
                                    // bit 1: 1 if extended colour information passed, 0 if not
                                    // bit 2: SMBIOS entry points passed.
                                    // All other bits are undefined and set to 0.
    // Extended colour information follows. Only access if bit 1 of flags is set.
    uint8_t  fb_memory_model;       // Memory model: 1=RGB, all other values undefined
    uint8_t  fb_red_mask_size;      // RGB mask sizes and left shifts
    uint8_t  fb_red_mask_shift;
    uint8_t  fb_green_mask_size;
    uint8_t  fb_green_mask_shift;
    uint8_t  fb_blue_mask_size;
    uint8_t  fb_blue_mask_shift;
    uint8_t  reserved;
    // Addresses of the SMBIOS entry points follow. Only access if bit 2 of flags is set.
    uint64_t smbios_entry_32;       // 0 if entry point unavailable
    uint64_t smbios_entry_64;       // 0 if entry point unavailable
} __attribute__((packed));
```

## Memory map entry

```c
struct mmap_entry {
    uint64_t base;      // Physical address of base of the memory section
    uint64_t length;    // Length of the section
    uint32_t type;      // Type (described below)
    uint32_t unused;
} __attribute__((packed));
```

`type` is an enumeration that can have the following values:

```
1      - Usable RAM
2      - Reserved
3      - ACPI reclaimable
4      - ACPI NVS
5      - Bad memory
10     - Kernel/Modules
0x1000 - Bootloader Reclaimable
0x1002 - Framebuffer
```

All other values are undefined.

The kernel and modules loaded **are not** marked as usable memory. They are marked
as Kernel/Modules (type 10).

The entries are guaranteed to be sorted by base address, lowest to highest.

Usable and bootloader reclaimable entries are guaranteed to be 4096 byte aligned for both base and length.

Usable and bootloader reclaimable entries are guaranteed not to overlap with any other entry.

To the contrary, all non-usable entries (including kernel/modules) are not guaranteed any alignment, nor
is it guaranteed that they do not overlap other entries.

## Modules

The `modules` variable points to the first entry of the linked list of module
structures.
A module structure looks like this:
```c
struct stivale_module {
    uint64_t begin;         // Address where the module is loaded
    uint64_t end;           // End address of the module
    char     string[128];   // 0-terminated ASCII string passed alongside the module
                            // (as specified in the config file)
    uint64_t next;          // Pointer to the next module (if any), check module_count
                            // in the stivale_struct
} __attribute__((packed));
```
