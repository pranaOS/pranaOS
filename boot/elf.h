#define ELF_MAGIC 0x464C457FU 

struct elfhdr
{
  uint32 magic; 
  uint8 elf[12];
  uint16 type;
  uint16 machine;
  uint32 version;
  uint64 entry;
  uint64 phoff;
  uint64 shoff;
  uint32 flags;
  uint16 ehsize;
  uint16 phentsize;
  uint16 phnum;
  uint16 shentsize;
  uint16 shnum;
  uint16 shstrndx;
};

struct proghdr
{
  uint32 type;
  uint32 flags;
  uint64 off;
  uint64 vaddr;
  uint64 paddr;
  uint64 filesz;
  uint64 memsz;
  uint64 align;
};

#define ELF_PROG_LOAD 1

#define ELF_PROG_FLAG_EXEC 1
#define ELF_PROG_FLAG_WRITE 2
#define ELF_PROG_FLAG_READ 4