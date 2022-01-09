#include <types.h>
#include <mem/mem.h>
#include <load.h>

using namespace bootloader;

#define SUPERBLOCK_POS 	0x0400
#define EXT2_SIGNATURE 	0xEF53
#define ROOT_INODE 		0x0002
#define INODE_SIZE 		0x0080
#define PLACEMENT_START 0xF000

unsigned int global_offset = 0;

#pragma pack(1)
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

#pragma pack(1)
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

#pragma pack(1)
typedef struct {
	uint32_t 		block_bitmap; 			
	uint32_t 		inode_bitmap;			
	uint32_t 		start_inode_table; 		
	uint16_t 		unallocated_block_count;
	uint16_t 		unallocated_inode_count;
	uint16_t 		directory_count;
	uint8_t			unused[14];
} bgd_t;

unsigned int _inode_offset_block(unsigned int inode_index, superblock_t *sb)
{
	return ((inode_index - 1) % sb->inodes_in_blockgroup * INODE_SIZE / (1024 << sb->block_size_frag));
}

unsigned int _inode_offset_rel_to_block(unsigned int inode_index, superblock_t *sb)
{
	return ((inode_index - 1) % sb->inodes_in_blockgroup % ((1024 << sb->block_size_frag) / INODE_SIZE)) * INODE_SIZE; 
}

inode_t *get_inode_ref(unsigned int inode_index, unsigned int startinode, unsigned int blocksize, superblock_t *sb, unsigned int disk)
{
	void *buf = malloc(1024 << sb->block_size_frag, 0);
	
	print("location: "); print_int(global_offset + _inode_offset_block(inode_index, sb) * ((1024 << sb->block_size_frag) / blocksize)); print("\n");
	
	load_block(disk, global_offset + (_inode_offset_block(inode_index, sb) + startinode) * ((1024 << sb->block_size_frag) / blocksize), (1024 << sb->block_size_frag) / blocksize, (unsigned int) buf, 0);
	
	return (inode_t*) (((unsigned int) buf) + _inode_offset_rel_to_block(inode_index, sb));
}

void load_ext2_block(unsigned int block, unsigned int placement_address, superblock_t *sb, unsigned int bs, unsigned int disk)
{
	load_block(disk, global_offset + block * ((1024 << sb->block_size_frag) / bs), (1024 << sb->block_size_frag) / bs, placement_address % 0x10, placement_address / 0x10);
}

unsigned int _handle_indirirect_bp(unsigned int indirect_bp_index, unsigned int block, superblock_t *sb, unsigned int bs, unsigned int disk)
{
	uint32_t *indirect_bp = (uint32_t*) malloc(1024 << sb->block_size_frag, 1);

	print(" "); print_int(indirect_bp);

	load_ext2_block(indirect_bp_index, (uint32_t) indirect_bp, sb, bs, disk);

	unsigned int ret = indirect_bp[block];

	free(0x1000);

	return ret;
}

unsigned int _ext2_get_block_loc(inode_t *inode, unsigned int block, superblock_t *sb, unsigned int blocksize, unsigned int disk)
{
	if (block < 12)
	{
		return inode->direct_block_pointer[block];
	}
	else
	{
		if (block < (1024 << sb->block_size_frag) / 4 + 12)
		{
			return _handle_indirirect_bp(inode->single_indirect_block_pointer, block - 12, sb, blocksize, disk);
		}
		else if (block < (unsigned int) pow((1024 << sb->block_size_frag) / 4, 2) + 12)
		{
			print("Double indirect\n");
			return 0;
		}
		else
		{
			return 0;
		}
	}
}

void _load_kernel(inode_t *kernel_inode, superblock_t *sb, unsigned int blocksize, unsigned int disk)
{
	unsigned int total_block_count = kernel_inode->size / (1024 << sb->block_size_frag);
	unsigned int placement_address = PLACEMENT_START;

	print("block count: "); print_int(total_block_count);

	unsigned int copy_iter = 0;

	print("Kernel size: "); print_int(kernel_inode->size); print("\n");

	for (size_t i = 0; i < total_block_count; i++)
	{
		unsigned int x = _ext2_get_block_loc(kernel_inode, i, sb, blocksize, disk);
		print(" "); print_int(x);
		if (x == 0)
		{
			print("Too large ?");
			break;
		}

		load_ext2_block(x, placement_address, sb, blocksize, disk);
		placement_address += (1024 << sb->block_size_frag);
		
		if (placement_address == (placement_address + 512 * 1024))
		{
			memcpy(KERNEL_ENTRY + (512 * 1024 * copy_iter), PLACEMENT_START, 512*1024);
			copy_iter++;
			placement_address = PLACEMENT_START;
		}
	}

	memcpy(KERNEL_ENTRY + (512 * 1024 * copy_iter), PLACEMENT_START, placement_address - PLACEMENT_START);
	print("placement_address: "); print_int(placement_address); print("\n");
}

int load_ext2_kernel(char *kernel_name, unsigned int startlba, unsigned int total_lba, unsigned int disk, unsigned int blocksize_phys)
{
	print("Loading kernel from ext2 filesystem\n");

	unsigned int superblock_lba = startlba + (SUPERBLOCK_POS / blocksize_phys);

	global_offset = startlba;
	
	superblock_t *sb = (superblock_t*) malloc(sizeof(superblock_t), 1);

	bgd_t *bgd = malloc(sizeof(bgd_t), 0);

	load_block(disk, superblock_lba, 1, (unsigned int) sb, 0);
	load_block(disk, superblock_lba + (1024 << sb->block_size_frag) / blocksize_phys, 1, (unsigned int) bgd, 0);

	print("Superblock loaded successfully\n");

	if (sb->ext2_signature != EXT2_SIGNATURE)
	{
		print("Cannot load kernel, not a ext2 filesystem or superblock was loaded incorrectly\n");
		return 1;
	}

	inode_t *inode_refrence = get_inode_ref(KERNEL_INODE, bgd->start_inode_table, blocksize_phys, sb, disk);
	
	print("Starting to load kernel\n");

	_load_kernel(inode_refrence, sb, blocksize_phys, disk);

}