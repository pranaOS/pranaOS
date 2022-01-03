#include <print_pm.h>
#include <types.h>
#include <fs/ext2.h>

#define PART_OFFSET (0x7c00 + 0x1BE)

#define FLAG_ACTIVE (1 << 7)

#define FS_TYPE_EXT2 0x01
#define FS_TYPE_FAT16 0x02
#define FS_TYPE_FAT32 0x03
#define FS_TYPE_EXT3 0x04 
#define FS_TYPE_EXT4 0x05

#define KERNEL_NAME "kernel"

void jump_kernel();

#pragma pack(1)
typedef struct partition_s
{
	uint8_t 	flags;
	uint8_t 	starting_head;	
	
	uint8_t 	start_sect;
	
	uint8_t 	starting_cyl;

	uint8_t 	system_id;
	uint8_t	 	ending_head;

	uint8_t 	ending_sect; 	
	
	uint8_t 	ending_cyl;

	uint32_t 	part_start_lba;
	uint32_t 	total_sects;
} partition_t;