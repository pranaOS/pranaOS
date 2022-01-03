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

typedef struct {
	uint16_t		type_permissions;	
	uint16_t 		uid;
	uint32_t 		size;				
	uint32_t 		last_access_time; 	
	uint32_t 		creation_time; 		
	uint32_t 		last_mod_time; 		
	uint32_t 		deletion_time; 		

	uint16_t		gid;
	
	uint16_t		hard_link_count;
	uint32_t 		disk_sectors; 		

	uint32_t 		flags;				
	uint32_t 		os_specific_value_1;

	uint32_t 		direct_block_pointer[12];
	uint32_t 		single_indirect_block_pointer;
	uint32_t 		double_indirect_block_pointer;
	uint32_t 		triple_indirect_block_pointer;

	uint32_t 		gen_number;
	uint32_t 		reserved;
	uint32_t 		reserved2;
	uint32_t 		block_addr_fragment;
	uint32_t 		os_specific_value_2[3]; 

} inode_t;

typedef struct {
	uint32_t 		block_bitmap; 			
	uint32_t 		inode_bitmap;			
	uint32_t 		start_inode_table; 		
	uint16_t 		unallocated_block_count;
	uint16_t 		unallocated_inode_count;
	uint16_t 		directory_count;
	uint8_t			unused[14];
} bgd_t;