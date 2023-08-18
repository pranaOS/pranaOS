/**
 * @file exec_elf.h
 * @author Krisna Pranav
 * @brief exec_elf
 * @version 6.0
 * @date 2023-08-06
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#ifndef _SYS_EXEC_ELF_H_
#define _SYS_EXEC_ELF_H_

#include <mods/types.h>

typedef uint8_t Elf_Byte;

typedef uint32_t Elf32_Addr; 
typedef uint32_t Elf32_Off;  
typedef int32_t Elf32_Sword; 
typedef uint32_t Elf32_Word; 
typedef uint16_t Elf32_Half; 
typedef uint64_t Elf32_Lword;

typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef int32_t Elf64_Shalf;

#ifdef __alpha__
typedef int64_t Elf64_Sword;
typedef uint64_t Elf64_Word;
#else
typedef int32_t Elf64_Sword;
typedef uint32_t Elf64_Word;
#endif

typedef int64_t Elf64_Sxword;
typedef uint64_t Elf64_Xword;
typedef uint64_t Elf64_Lword;

typedef uint32_t Elf64_Half;
typedef uint16_t Elf64_Quarter;

#define EI_MAG0 0       
#define EI_MAG1 1       
#define EI_MAG2 2       
#define EI_MAG3 3       
#define EI_CLASS 4      
#define EI_DATA 5       
#define EI_VERSION 6    
#define EI_OSABI 7      
#define EI_ABIVERSION 8 
#define EI_PAD 9        
#define EI_NIDENT 16    

#define ELFMAG0 0x7f     
#define ELFMAG1 'E'      
#define ELFMAG2 'L'      
#define ELFMAG3 'F'      
#define ELFMAG "\177ELF" 
#define SELFMAG 4        

#define ELFCLASSNONE 0 
#define ELFCLASS32 1   
#define ELFCLASS64 2   
#define ELFCLASSNUM 3  

#define ELFDATANONE 0 
#define ELFDATA2LSB 1 
#define ELFDATA2MSB 2 
#define ELFDATANUM 3  

#define ELFOSABI_SYSV 0         
#define ELFOSABI_HPUX 1         
#define ELFOSABI_NETBSD 2       
#define ELFOSABI_LINUX 3        
#define ELFOSABI_HURD 4         
#define ELFOSABI_86OPEN 5       
#define ELFOSABI_SOLARIS 6      
#define ELFOSABI_MONTEREY 7     
#define ELFOSABI_IRIX 8         
#define ELFOSABI_FREEBSD 9      
#define ELFOSABI_TRU64 10       
#define ELFOSABI_MODESTO 11     
#define ELFOSABI_OPENBSD 12     
#define ELFOSABI_ARM 97         
#define ELFOSABI_STANDALONE 255 

#define IS_ELF(ehdr) ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && (ehdr).e_ident[EI_MAG1] == ELFMAG1 && (ehdr).e_ident[EI_MAG2] == ELFMAG2 && (ehdr).e_ident[EI_MAG3] == ELFMAG3)

typedef struct elfhdr 
{
    unsigned char e_ident[EI_NIDENT]; 
    Elf32_Half e_type;                
    Elf32_Half e_machine;             
    Elf32_Word e_version;             
    Elf32_Addr e_entry;               
    Elf32_Off e_phoff;                
    Elf32_Off e_shoff;                
    Elf32_Word e_flags;               
    Elf32_Half e_ehsize;              
    Elf32_Half e_phentsize;           
    Elf32_Half e_phnum;               
    Elf32_Half e_shentsize;           
    Elf32_Half e_shnum;               
    Elf32_Half e_shstrndx;            
} Elf32_Ehdr;

typedef struct 
{
    unsigned char e_ident[EI_NIDENT]; 
    Elf64_Quarter e_type;             
    Elf64_Quarter e_machine;          
    Elf64_Half e_version;             
    Elf64_Addr e_entry;               
    Elf64_Off e_phoff;                
    Elf64_Off e_shoff;                
    Elf64_Half e_flags;               
    Elf64_Quarter e_ehsize;           
    Elf64_Quarter e_phentsize;        
    Elf64_Quarter e_phnum;            
    Elf64_Quarter e_shentsize;        
    Elf64_Quarter e_shnum;            
    Elf64_Quarter e_shstrndx;         
} Elf64_Ehdr;

#define ET_NONE 0        
#define ET_REL 1         
#define ET_EXEC 2        
#define ET_DYN 3         
#define ET_CORE 4        
#define ET_NUM 5         
#define ET_LOPROC 0xff00 
#define ET_HIPROC 0xffff 

#define EM_NONE 0  
#define EM_M32 1   
#define EM_SPARC 2 
#define EM_386 3   
#define EM_68K 4   
#define EM_88K 5   
#define EM_486 6   
#define EM_860 7   
#define EM_MIPS 8  

#define EM_MIPS_RS4_BE 10 
#define EM_SPARC64 11     
#define EM_PARISC 15      
#define EM_SPARC32PLUS 18 
#define EM_PPC 20         
#define EM_PPC64 21       
#define EM_ARM 40         
#define EM_ALPHA 41       
#define EM_SH 42          
#define EM_SPARCV9 43     
#define EM_IA_64 50       
#define EM_AMD64 62       
#define EM_X86_64 EM_AMD64
#define EM_VAX 75      
#define EM_AARCH64 183 

#define EM_ALPHA_EXP 0x9026 
#define EM__LAST__ (EM_ALPHA_EXP + 1)

#define EM_NUM 22 

#define EV_NONE 0    
#define EV_CURRENT 1 
#define EV_NUM 2     

#define PN_XNUM 0xffff

typedef struct 
{
    Elf32_Word sh_name;      
    Elf32_Word sh_type;      
    Elf32_Word sh_flags;     
    Elf32_Addr sh_addr;      
    Elf32_Off sh_offset;     
    Elf32_Word sh_size;      
    Elf32_Word sh_link;      
    Elf32_Word sh_info;      
    Elf32_Word sh_addralign; 
    Elf32_Word sh_entsize;   
} Elf32_Shdr;

typedef struct 
{
    Elf64_Half sh_name;       
    Elf64_Half sh_type;       
    Elf64_Xword sh_flags;     
    Elf64_Addr sh_addr;       
    Elf64_Off sh_offset;      
    Elf64_Xword sh_size;      
    Elf64_Half sh_link;       
    Elf64_Half sh_info;       
    Elf64_Xword sh_addralign; 
    Elf64_Xword sh_entsize;   
} Elf64_Shdr;

#define SHN_UNDEF 0          
#define SHN_LORESERVE 0xff00 
#define SHN_LOPROC 0xff00    
#define SHN_HIPROC 0xff1f    
#define SHN_ABS 0xfff1       
#define SHN_COMMON 0xfff2    
#define SHN_XINDEX 0xffff    
#define SHN_HIRESERVE 0xffff 

#define SHT_NULL 0                 
#define SHT_PROGBITS 1              
#define SHT_SYMTAB 2                
#define SHT_STRTAB 3                
#define SHT_RELA 4                  
#define SHT_HASH 5                  
#define SHT_DYNAMIC 6               
#define SHT_NOTE 7                  
#define SHT_NOBITS 8                
#define SHT_REL 9                   
#define SHT_SHLIB 10                
#define SHT_DYNSYM 11               
#define SHT_NUM 12                  
#define SHT_INIT_ARRAY 14           
#define SHT_FINI_ARRAY 15           
#define SHT_PREINIT_ARRAY 16        
#define SHT_GROUP 17                
#define SHT_SYMTAB_SHNDX 18         
#define SHT_LOOS 0x60000000         
#define SHT_SUNW_dof 0x6ffffff4     
#define SHT_GNU_LIBLIST 0x6ffffff7  
#define SHT_SUNW_move 0x6ffffffa    
#define SHT_SUNW_syminfo 0x6ffffffc 
#define SHT_SUNW_verdef 0x6ffffffd  
#define SHT_SUNW_verneed 0x6ffffffe 
#define SHT_SUNW_versym 0x6fffffff  
#define SHT_HIOS 0x6fffffff         
#define SHT_LOPROC 0x70000000       
#define SHT_HIPROC 0x7fffffff       
#define SHT_LOUSER 0x80000000       
#define SHT_HIUSER 0xffffffff       

#define SHT_GNU_HASH 0x6ffffff6 

#define ELF_BSS ".bss"                              
#define ELF_DATA ".data"                            
#define ELF_CTF ".SUNW_ctf"                         
#define ELF_DEBUG ".debug"                          
#define ELF_DYNAMIC ".dynamic"                      
#define ELF_DYNSTR ".dynstr"                        
#define ELF_DYNSYM ".dynsym"                        
#define ELF_FINI ".fini"                            
#define ELF_GOT ".got"                              
#define ELF_HASH ".hash"                            
#define ELF_INIT ".init"                            
#define ELF_REL_DATA ".rel.data"                    
#define ELF_REL_FINI ".rel.fini"                    
#define ELF_REL_INIT ".rel.init"                    
#define ELF_REL_DYN ".rel.dyn"                      
#define ELF_REL_RODATA ".rel.rodata"                
#define ELF_REL_TEXT ".rel.text"                    
#define ELF_RODATA ".rodata"                        
#define ELF_SHSTRTAB ".shstrtab"                    
#define ELF_STRTAB ".strtab"                        
#define ELF_SYMTAB ".symtab"                        
#define ELF_TEXT ".text"                            
#define ELF_OPENBSDRANDOMDATA ".openbsd.randomdata" 

#define SHF_WRITE 0x1              
#define SHF_ALLOC 0x2              
#define SHF_EXECINSTR 0x4          
#define SHF_MERGE 0x10             
#define SHF_STRINGS 0x20           
#define SHF_INFO_LINK 0x40         
#define SHF_LINK_ORDER 0x80        
#define SHF_OS_NONCONFORMING 0x100 
#define SHF_GROUP 0x200            
#define SHF_TLS 0x400              
#define SHF_COMPRESSED 0x800       
#define SHF_MASKOS 0x0ff00000      
#define SHF_MASKPROC 0xf0000000    
typedef struct elf32_sym 
{
    Elf32_Word st_name;     
    Elf32_Addr st_value;    
    Elf32_Word st_size;     
    unsigned char st_info;  
    unsigned char st_other; 
    Elf32_Half st_shndx;    
} Elf32_Sym;

typedef struct 
{
    Elf64_Half st_name;     
    Elf_Byte st_info;       
    Elf_Byte st_other;      
    Elf64_Quarter st_shndx; 
    Elf64_Xword st_value;   
    Elf64_Xword st_size;    
} Elf64_Sym;

#define STN_UNDEF 0 

#define ELF32_ST_BIND(x) ((x) >> 4)
#define ELF32_ST_TYPE(x) (((unsigned int)x) & 0xf)
#define ELF32_ST_INFO(b, t) (((b) << 4) + ((t)&0xf))

#define ELF64_ST_BIND(x) ((x) >> 4)
#define ELF64_ST_TYPE(x) (((unsigned int)x) & 0xf)
#define ELF64_ST_INFO(b, t) (((b) << 4) + ((t)&0xf))

#define STB_LOCAL 0   
#define STB_GLOBAL 1  
#define STB_WEAK 2    
#define STB_NUM 3     
#define STB_LOPROC 13 
#define STB_HIPROC 15 

#define STT_NOTYPE 0  
#define STT_OBJECT 1  
#define STT_FUNC 2    
#define STT_SECTION 3 
#define STT_FILE 4    
#define STT_TLS 6     
#define STT_LOPROC 13 
#define STT_HIPROC 15 

#define ELF_ST_VISIBILITY(v) ((v)&0x3)
#define ELF32_ST_VISIBILITY ELF_ST_VISIBILITY
#define ELF64_ST_VISIBILITY ELF_ST_VISIBILITY

#define STV_DEFAULT 0   
#define STV_INTERNAL 1  
#define STV_HIDDEN 2    
#define STV_PROTECTED 3 

typedef struct 
{
    Elf32_Addr r_offset; 
    Elf32_Word r_info;   
} Elf32_Rel;

typedef struct 
{
    Elf32_Addr r_offset; 
    Elf32_Word r_info;   
    Elf32_Sword r_addend;
} Elf32_Rela;

#define ELF32_R_SYM(i) ((i) >> 8)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s, t) (((s) << 8) + (unsigned char)(t))

typedef struct 
{
    Elf64_Xword r_offset;
    Elf64_Xword r_info;   
} Elf64_Rel;

typedef struct 
{
    Elf64_Xword r_offset;  
    Elf64_Xword r_info;    
    Elf64_Sxword r_addend; 
} Elf64_Rela;

#define ELF64_R_SYM(info) ((info) >> 32)
#define ELF64_R_TYPE(info) ((info)&0xFFFFFFFF)
#define ELF64_R_INFO(s, t) (((s) << 32) + (uint32_t)(t))

#if defined(__mips64__) && defined(__MIPSEL__)
#    undef ELF64_R_SYM
#    undef ELF64_R_TYPE
#    undef ELF64_R_INFO
#    define ELF64_R_TYPE(info) ((uint64_t)swap32((info) >> 32))
#    define ELF64_R_SYM(info) ((info)&0xFFFFFFFF)
#    define ELF64_R_INFO(s, t) (((uint64_t)swap32(t) << 32) + (uint32_t)(s))
#endif 

typedef struct 
{
    Elf32_Word p_type;   
    Elf32_Off p_offset;  
    Elf32_Addr p_vaddr;  
    Elf32_Addr p_paddr;  
    Elf32_Word p_filesz; 
    Elf32_Word p_memsz;  
    Elf32_Word p_flags;  
    Elf32_Word p_align;  
} Elf32_Phdr;

typedef struct 
{
    Elf64_Half p_type;    
    Elf64_Half p_flags;   
    Elf64_Off p_offset;   
    Elf64_Addr p_vaddr;   
    Elf64_Addr p_paddr;   
    Elf64_Xword p_filesz; 
    Elf64_Xword p_memsz;  
    Elf64_Xword p_align;  
} Elf64_Phdr;

#define PT_NULL 0            
#define PT_LOAD 1            
#define PT_DYNAMIC 2         
#define PT_INTERP 3          
#define PT_NOTE 4            
#define PT_SHLIB 5           
#define PT_PHDR 6            
#define PT_TLS 7             
#define PT_LOOS 0x60000000   
#define PT_HIOS 0x6fffffff   
#define PT_LOPROC 0x70000000 
#define PT_HIPROC 0x7fffffff 

#define PT_GNU_EH_FRAME 0x6474e550 
#define PT_GNU_RELRO 0x6474e552    
#define PT_GNU_STACK 0x6474e551    

#define PT_OPENBSD_RANDOMIZE 0x65a3dbe6 
#define PT_OPENBSD_WXNEEDED 0x65a3dbe7  
#define PT_OPENBSD_BOOTDATA 0x65a41be6  

#define PF_X 0x1               
#define PF_W 0x2               
#define PF_R 0x4               
#define PF_MASKPROC 0xf0000000 
                               

typedef struct 
{
    Elf32_Sword d_tag; 
    union 
    {
        Elf32_Word d_val; 
        Elf32_Addr d_ptr; 
    } d_un;
} Elf32_Dyn;

typedef struct 
{
    Elf64_Xword d_tag; 
    union 
    {
        Elf64_Addr d_ptr;
        Elf64_Xword d_val;
    } d_un;
} Elf64_Dyn;

#define DT_NULL 0             
#define DT_NEEDED 1           
#define DT_PLTRELSZ 2         
#define DT_PLTGOT 3           
#define DT_HASH 4             
#define DT_STRTAB 5           
#define DT_SYMTAB 6           
#define DT_RELA 7             
#define DT_RELASZ 8           
#define DT_RELAENT 9          
#define DT_STRSZ 10           
#define DT_SYMENT 11          
#define DT_INIT 12            
#define DT_FINI 13            
#define DT_SONAME 14          
#define DT_RPATH 15           
#define DT_SYMBOLIC 16        
#define DT_REL 17             
#define DT_RELSZ 18           
#define DT_RELENT 19          
#define DT_PLTREL 20          
#define DT_DEBUG 21           
#define DT_TEXTREL 22         
#define DT_JMPREL 23          
#define DT_BIND_NOW 24        
#define DT_INIT_ARRAY 25      
#define DT_FINI_ARRAY 26      
#define DT_INIT_ARRAYSZ 27    
#define DT_FINI_ARRAYSZ 28    
#define DT_RUNPATH 29         
#define DT_FLAGS 30           
#define DT_ENCODING 31        
#define DT_PREINIT_ARRAY 32   
#define DT_PREINIT_ARRAYSZ 33 
#define DT_LOOS 0x6000000d    
#define DT_HIOS 0x6ffff000    
#define DT_LOPROC 0x70000000  
#define DT_HIPROC 0x7fffffff  

#define DT_GNU_HASH 0x6ffffef5  
#define DT_RELACOUNT 0x6ffffff9 
#define DT_RELCOUNT 0x6ffffffa  
#define DT_FLAGS_1 0x6ffffffb
#define DF_ORIGIN 0x00000001
#define DF_SYMBOLIC 0x00000002
#define DF_TEXTREL 0x00000004
#define DF_BIND_NOW 0x00000008
#define DF_STATIC_TLS 0x00000010
#define DF_1_NOW 0x00000001
#define DF_1_GLOBAL 0x00000002
#define DF_1_GROUP 0x00000004
#define DF_1_NODELETE 0x00000008
#define DF_1_LOADFLTR 0x00000010
#define DF_1_INITFIRST 0x00000020
#define DF_1_NOOPEN 0x00000040
#define DF_1_ORIGIN 0x00000080
#define DF_1_DIRECT 0x00000100
#define DF_1_TRANS 0x00000200
#define DF_1_INTERPOSE 0x00000400
#define DF_1_NODEFLIB 0x00000800
#define DF_1_NODUMP 0x00001000
#define DF_1_CONLFAT 0x00002000

typedef struct 
{
    Elf32_Word n_namesz;
    Elf32_Word n_descsz;
    Elf32_Word n_type;
} Elf32_Nhdr;

typedef struct 
{
    Elf64_Half n_namesz;
    Elf64_Half n_descsz;
    Elf64_Half n_type;
} Elf64_Nhdr;

typedef struct 
{
    Elf32_Word namesz;
    Elf32_Word descsz;
    Elf32_Word type;
} Elf32_Note;

typedef struct 
{
    Elf64_Half namesz;
    Elf64_Half descsz;
    Elf64_Half type;
} Elf64_Note;

#define NT_PRSTATUS 1 
#define NT_FPREGSET 2 
#define NT_PRPSINFO 3 

#define NT_OPENBSD_PROCINFO 10
#define NT_OPENBSD_AUXV 11

#define NT_OPENBSD_REGS 20
#define NT_OPENBSD_FPREGS 21
#define NT_OPENBSD_XFPREGS 22
#define NT_OPENBSD_WCOOKIE 23

struct elfcore_procinfo 
{
    uint32_t cpi_version;
#define ELFCORE_PROCINFO_VERSION 1
    uint32_t cpi_cpisize;   
    uint32_t cpi_signo;     
    uint32_t cpi_sigcode;   
    uint32_t cpi_sigpend;   
    uint32_t cpi_sigmask;   
    uint32_t cpi_sigignore; 
    uint32_t cpi_sigcatch;  
    int32_t cpi_pid;        
    int32_t cpi_ppid;       
    int32_t cpi_pgrp;       
    int32_t cpi_sid;        
    uint32_t cpi_ruid;      
    uint32_t cpi_euid;      
    uint32_t cpi_svuid;     
    uint32_t cpi_rgid;      
    uint32_t cpi_egid;      
    uint32_t cpi_svgid;     
    int8_t cpi_name[32];    
};

#if defined(_KERNEL) || defined(_DYN_LOADER)

#    define ELF32_NO_ADDR ((uint32_t)~0) 

typedef struct 
{
    Elf32_Sword au_id; 
    Elf32_Word au_v;   
} Aux32Info;

#    define ELF64_NO_ADDR ((uint64_t)~0) 

typedef struct 
{
    Elf64_Shalf au_id; 
    Elf64_Xword au_v;  
} Aux64Info;

enum AuxID 
{
    AUX_null = 0,
    AUX_ignore = 1,
    AUX_execfd = 2,
    AUX_phdr = 3,        
    AUX_phent = 4,       
    AUX_phnum = 5,       
    AUX_pagesz = 6,      
    AUX_base = 7,        
    AUX_flags = 8,       
    AUX_entry = 9,       
    AUX_sun_uid = 2000,  
    AUX_sun_ruid = 2001, 
    AUX_sun_gid = 2002,  
    AUX_sun_rgid = 2003  
};

struct elf_args 
{
    u_long arg_entry;     
    u_long arg_interp;    
    u_long arg_phaddr;    
    u_long arg_phentsize; 
    u_long arg_phnum;     
};

#endif

#if !defined(ELFSIZE) && defined(ARCH_ELFSIZE)
#    define ELFSIZE ARCH_ELFSIZE
#endif

#if defined(ELFSIZE)
#    define CONCAT(x, y) __CONCAT(x, y)
#    define ELFNAME(x) CONCAT(elf, CONCAT(ELFSIZE, CONCAT(_, x)))
#    define ELFDEFNNAME(x) CONCAT(ELF, CONCAT(ELFSIZE, CONCAT(_, x)))
#endif

#if defined(ELFSIZE) && (ELFSIZE == 32)
#    define Elf_Ehdr Elf32_Ehdr
#    define Elf_Phdr Elf32_Phdr
#    define Elf_Shdr Elf32_Shdr
#    define Elf_Sym Elf32_Sym
#    define Elf_Rel Elf32_Rel
#    define Elf_RelA Elf32_Rela
#    define Elf_Dyn Elf32_Dyn
#    define Elf_Half Elf32_Half
#    define Elf_Word Elf32_Word
#    define Elf_Sword Elf32_Sword
#    define Elf_Addr Elf32_Addr
#    define Elf_Off Elf32_Off
#    define Elf_Nhdr Elf32_Nhdr
#    define Elf_Note Elf32_Note

#    define ELF_R_SYM ELF32_R_SYM
#    define ELF_R_TYPE ELF32_R_TYPE
#    define ELF_R_INFO ELF32_R_INFO
#    define ELFCLASS ELFCLASS32

#    define ELF_ST_BIND ELF32_ST_BIND
#    define ELF_ST_TYPE ELF32_ST_TYPE
#    define ELF_ST_INFO ELF32_ST_INFO

#    define ELF_NO_ADDR ELF32_NO_ADDR
#    define AuxInfo Aux32Info
#elif defined(ELFSIZE) && (ELFSIZE == 64)
#    define Elf_Ehdr Elf64_Ehdr
#    define Elf_Phdr Elf64_Phdr
#    define Elf_Shdr Elf64_Shdr
#    define Elf_Sym Elf64_Sym
#    define Elf_Rel Elf64_Rel
#    define Elf_RelA Elf64_Rela
#    define Elf_Dyn Elf64_Dyn
#    define Elf_Half Elf64_Half
#    define Elf_Word Elf64_Word
#    define Elf_Sword Elf64_Sword
#    define Elf_Addr Elf64_Addr
#    define Elf_Off Elf64_Off
#    define Elf_Nhdr Elf64_Nhdr
#    define Elf_Note Elf64_Note

#    define ELF_R_SYM ELF64_R_SYM
#    define ELF_R_TYPE ELF64_R_TYPE
#    define ELF_R_INFO ELF64_R_INFO
#    define ELFCLASS ELFCLASS64

#    define ELF_ST_BIND ELF64_ST_BIND
#    define ELF_ST_TYPE ELF64_ST_TYPE
#    define ELF_ST_INFO ELF64_ST_INFO

#    define ELF_NO_ADDR ELF64_NO_ADDR
#    define AuxInfo Aux64Info
#endif

#define ELF_TARG_VER 1 
#define R_386_NONE 0
#define R_386_32 1         
#define R_386_PC32 2       
#define R_386_GOT32 3      
#define R_386_PLT32 4      
#define R_386_COPY 5       
#define R_386_GLOB_DAT 6   
#define R_386_JMP_SLOT 7   
#define R_386_RELATIVE 8   
#define R_386_TLS_TPOFF 14 
#define R_386_TLS_TPOFF32 37

#endif 
