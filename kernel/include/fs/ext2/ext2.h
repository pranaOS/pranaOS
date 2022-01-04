#pragma once

#include <fs/fs.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
	uint32_t total_inodes;
	uint32_t total_blocks;
	uint32_t total_reserved_for_superuser;
	uint32_t total_unallocated_blocks;
	uint32_t total_unallocated_inodes;
	uint32_t block_number_of_superblock;

	uint32_t block_size_frag; 
	                          
	uint32_t fragment_size_frag; 

	uint32_t blocks_in_blockgroup;
	uint32_t fragments_in_blockgroup;
	uint32_t inodes_in_blockgroup;
	uint32_t last_mount_time;   
	uint32_t last_written_time; 

	uint16_t mount_amount_since_last_check; 
	uint16_t max_mounts_after_check;

	uint16_t ext2_signature; 
	                         
	uint16_t fs_state;      
	uint16_t
		when_error; 
	uint16_t minor_version;

	uint32_t last_check_time;       
	uint32_t forced_check_interval; 
	uint32_t os_creator_id; 
	                        
	uint32_t major_version;

	uint16_t uid;
	uint16_t gid;

	uint32_t first_non_reserved_inode;
	uint16_t inode_size;
	uint16_t block_group_of_this_superblock;

	uint32_t optional_feature_flag;
	uint32_t required_feature_flag;
	uint32_t
		must_required_feature_flag; 

	uint8_t fs_id[16];           
	uint8_t volume_name[16];     
	uint8_t last_mount_path[64]; 

	uint32_t compression_algorithms;

	uint8_t blocks_to_preallocate_files;
	uint8_t blocks_to_preallocate_dirs;

	uint16_t UNUSED;

	uint8_t  journal_id[16]; 
	uint32_t journal_inode;
	uint32_t journal_device;
	uint32_t orphan_inode_list_head;

} __attribute__((packed)) ext2_superblock_t;

typedef struct {
    uint32_t block_bitmap;
    uint32_t inode_bitmap;
    uint32_t start_inode_table;
    uint32_t unallocated_block_count;
    uint16_t unallocated_inode_count;
    uint16_t directory_count;
    uint8_t unused[14];
} __attribute__((packed)) ext2_block_group_descriptor_t;

typedef struct {

} __attribute__((packed)) ext2_inode_t;

typedef struct {

} __attribute__((packed)) ext2_directory_entry_t;

filesystem_t *init_ext2_filesystem(char *name, unsigned int ext2_start, disk_t *disk_info);