#include <types.h>
#include <mem/mem.h>
#include <load.h>

#define SUPERBLOCK_POS 0x0400
#define EXT2_SIGNATURE 0xEF53
#define ROOT_INODE 0x0002
#define INODE_SIZE 0x0080
#define PLACEMENT_START 0xF000

unsigned int global_offset = 0;

typedef struct {
	uint32_t 		total_inodes;
	uint32_t 		total_blocks;
	uint32_t 		total_reserved_for_superuser;
	uint32_t 		total_unallocated_blocks;
	uint32_t 		total_unallocated_inodes;
	uint32_t 		block_number_of_superblock;
	
	uint32_t 		block_size_frag;		
	uint32_t 		fragment_size_frag;		

	uint32_t 		blocks_in_blockgroup;
	uint32_t 		fragments_in_blockgroup;
	uint32_t 		inodes_in_blockgroup;
	uint32_t 		last_mount_time;		
	uint32_t 		last_written_time;		

	uint16_t		mount_amount_since_last_check;
	uint16_t		max_mounts_after_check;

	uint16_t		ext2_signature;
	uint16_t		fs_state;		
	uint16_t		when_error; 	
	uint16_t		minor_version;

	uint32_t		last_check_time;			
	uint32_t		forced_check_interval;		
	uint32_t		os_creator_id;				
	uint32_t		major_version;

	uint16_t		uid;
	uint16_t		gid;
} superblock_t;