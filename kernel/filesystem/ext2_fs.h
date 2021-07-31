/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _LINUX_EXT2_FS_H
#define _LINUX_EXT2_FS_H

#include "ext2_types.h" 

#define EXT2_PREALLOCATE
#define EXT2_DEFAULT_PREALLOC_BLOCKS 8

#define EXT2FS_DATE "95/08/09"
#define EXT2FS_VERSION "0.5b"


#define EXT2_BAD_INO 1         
#define EXT2_ROOT_INO 2        
#define EXT2_ACL_IDX_INO 3     
#define EXT2_ACL_DATA_INO 4    
#define EXT2_BOOT_LOADER_INO 5 
#define EXT2_UNDEL_DIR_INO 6   
#define EXT2_RESIZE_INO 7      
#define EXT2_JOURNAL_INO 8     

#define EXT2_GOOD_OLD_FIRST_INO 11

#define EXT2_SUPER_MAGIC 0xEF53

#ifdef __KERNEL__
#    define EXT2_SB(sb) (&((sb)->u.ext2_sb))
#else

#    define EXT2_SB(sb) (sb)
#endif

#define EXT2_LINK_MAX 65000

#define EXT2_MIN_BLOCK_LOG_SIZE 10 
#define EXT2_MAX_BLOCK_LOG_SIZE 16 
#define EXT2_MIN_BLOCK_SIZE (1 << EXT2_MIN_BLOCK_LOG_SIZE)
#define EXT2_MAX_BLOCK_SIZE (1 << EXT2_MAX_BLOCK_LOG_SIZE)
#ifdef __KERNEL__
#    define EXT2_BLOCK_SIZE(s) ((s)->s_blocksize)
#    define EXT2_BLOCK_SIZE_BITS(s) ((s)->s_blocksize_bits)
#    define EXT2_ADDR_PER_BLOCK_BITS(s) (EXT2_SB(s)->addr_per_block_bits)
#    define EXT2_INODE_SIZE(s) (EXT2_SB(s)->s_inode_size)
#    define EXT2_FIRST_INO(s) (EXT2_SB(s)->s_first_ino)
#else
#    define EXT2_BLOCK_SIZE(s) (EXT2_MIN_BLOCK_SIZE << (s)->s_log_block_size)
#    define EXT2_BLOCK_SIZE_BITS(s) ((s)->s_log_block_size + 10)
#    define EXT2_INODE_SIZE(s) (((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? EXT2_GOOD_OLD_INODE_SIZE : (s)->s_inode_size)
#    define EXT2_FIRST_INO(s) (((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? EXT2_GOOD_OLD_FIRST_INO : (s)->s_first_ino)
#endif
#define EXT2_ADDR_PER_BLOCK(s) (EXT2_BLOCK_SIZE(s) / sizeof(__u32))


#define EXT2_MIN_FRAG_SIZE EXT2_MIN_BLOCK_SIZE
#define EXT2_MAX_FRAG_SIZE EXT2_MAX_BLOCK_SIZE
#define EXT2_MIN_FRAG_LOG_SIZE EXT2_MIN_BLOCK_LOG_SIZE
#ifdef __KERNEL__
#    define EXT2_FRAG_SIZE(s) (EXT2_SB(s)->s_frag_size)
#    define EXT2_FRAGS_PER_BLOCK(s) (EXT2_SB(s)->s_frags_per_block)
#else
#    define EXT2_FRAG_SIZE(s) (EXT2_MIN_FRAG_SIZE << (s)->s_log_frag_size)
#    define EXT2_FRAGS_PER_BLOCK(s) (EXT2_BLOCK_SIZE(s) / EXT2_FRAG_SIZE(s))
#endif

struct ext2_acl_header 
{
    __u32 aclh_size;
    __u32 aclh_file_count;
    __u32 aclh_acle_count;
    __u32 aclh_first_acle;
};

struct ext2_acl_entry 
{
    __u32 acle_size;
    __u16 acle_perms; 
    __u16 acle_type;  
    __u16 acle_tag;   
    __u16 acle_pad1;
    __u32 acle_next; 
};

struct ext2_group_desc {
    __u32 bg_block_bitmap;      
    __u32 bg_inode_bitmap;      
    __u32 bg_inode_table;       
    __u16 bg_free_blocks_count; 
    __u16 bg_free_inodes_count; 
    __u16 bg_used_dirs_count;   
    __u16 bg_flags;
    __u32 bg_reserved[2];
    __u16 bg_itable_unused; 
    __u16 bg_checksum;      
};

struct ext4_group_desc {
    __u32 bg_block_bitmap;      
    __u32 bg_inode_bitmap;      
    __u32 bg_inode_table;       
    __u16 bg_free_blocks_count; 
    __u16 bg_free_inodes_count; 
    __u16 bg_used_dirs_count;   
    __u16 bg_flags;
    __u32 bg_reserved[2];
    __u16 bg_itable_unused;         
    __u16 bg_checksum;              
    __u32 bg_block_bitmap_hi;       
    __u32 bg_inode_bitmap_hi;       
    __u32 bg_inode_table_hi;        
    __u16 bg_free_blocks_count_hi;  
    __u16 bg_free_inodes_count_hi;  
    __u16 bg_used_dirs_count_hi;    
    __u16 bg_pad;
    __u32 bg_reserved2[3];
};

#define EXT2_BG_INODE_UNINIT 0x0001 
#define EXT2_BG_BLOCK_UNINIT 0x0002 
#define EXT2_BG_INODE_ZEROED 0x0004 

struct ext2_dx_root_info {
    __u32 reserved_zero;
    __u8 hash_version;  
    __u8 info_length;   
    __u8 indirect_levels;
    __u8 unused_flags;
};

#define EXT2_HASH_LEGACY 0
#define EXT2_HASH_HALF_MD4 1
#define EXT2_HASH_TEA 2
#define EXT2_HASH_LEGACY_UNSIGNED 3   
#define EXT2_HASH_HALF_MD4_UNSIGNED 4 
#define EXT2_HASH_TEA_UNSIGNED 5      

#define EXT2_HASH_FLAG_INCOMPAT 0x1

struct ext2_dx_entry {
    __u32 hash;
    __u32 block;
};

struct ext2_dx_countlimit {
    __u16 limit;
    __u16 count;
};

#define EXT2_MIN_DESC_SIZE 32
#define EXT2_MIN_DESC_SIZE_64BIT 64
#define EXT2_MAX_DESC_SIZE EXT2_MIN_BLOCK_SIZE
#define EXT2_DESC_SIZE(s) \
    ((EXT2_SB(s)->s_feature_incompat & EXT4_FEATURE_INCOMPAT_64BIT) ? (s)->s_desc_size : EXT2_MIN_DESC_SIZE)

#define EXT2_BLOCKS_PER_GROUP(s) (EXT2_SB(s)->s_blocks_per_group)
#define EXT2_INODES_PER_GROUP(s) (EXT2_SB(s)->s_inodes_per_group)
#define EXT2_INODES_PER_BLOCK(s) (EXT2_BLOCK_SIZE(s) / EXT2_INODE_SIZE(s))

#define EXT2_MAX_BLOCKS_PER_GROUP(s) ((1 << 16) - 8)
#define EXT2_MAX_INODES_PER_GROUP(s) ((1 << 16) - EXT2_INODES_PER_BLOCK(s))
#ifdef __KERNEL__
#    define EXT2_DESC_PER_BLOCK(s) (EXT2_SB(s)->s_desc_per_block)
#    define EXT2_DESC_PER_BLOCK_BITS(s) (EXT2_SB(s)->s_desc_per_block_bits)
#else
#    define EXT2_DESC_PER_BLOCK(s) (EXT2_BLOCK_SIZE(s) / EXT2_DESC_SIZE(s))
#endif


#define EXT2_NDIR_BLOCKS 12
#define EXT2_IND_BLOCK EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK (EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK (EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS (EXT2_TIND_BLOCK + 1)

#define EXT2_SECRM_FL 0x00000001     
#define EXT2_UNRM_FL 0x00000002      
#define EXT2_COMPR_FL 0x00000004     
#define EXT2_SYNC_FL 0x00000008      
#define EXT2_IMMUTABLE_FL 0x00000010 
#define EXT2_APPEND_FL 0x00000020    
#define EXT2_NODUMP_FL 0x00000040    
#define EXT2_NOATIME_FL 0x00000080   

#define EXT2_DIRTY_FL 0x00000100
#define EXT2_COMPRBLK_FL 0x00000200 
#define EXT2_NOCOMPR_FL 0x00000400  
#define EXT2_ECOMPR_FL 0x00000800   

#define EXT2_BTREE_FL 0x00001000 
#define EXT2_INDEX_FL 0x00001000 
#define EXT2_IMAGIC_FL 0x00002000
#define EXT3_JOURNAL_DATA_FL 0x00004000 
#define EXT2_NOTAIL_FL 0x00008000       
#define EXT2_DIRSYNC_FL 0x00010000      
#define EXT2_TOPDIR_FL 0x00020000       
#define EXT4_HUGE_FILE_FL 0x00040000    
#define EXT4_EXTENTS_FL 0x00080000      
#define EXT2_RESERVED_FL 0x80000000     

#define EXT2_FL_USER_VISIBLE 0x000BDFFF     
#define EXT2_FL_USER_MODIFIABLE 0x000080FF  

struct ext2_new_group_input {
    __u32 group;            
    __u32 block_bitmap;     
    __u32 inode_bitmap;     
    __u32 inode_table;      
    __u32 blocks_count;     
    __u16 reserved_blocks;  
    __u16 unused;           
};

struct ext4_new_group_input {
    __u32 group;            
    __u64 block_bitmap;     
    __u64 inode_bitmap;     
    __u64 inode_table;      
    __u32 blocks_count;    
    __u16 reserved_blocks;  
    __u16 unused;
};

#ifdef __GNU__ 
#    define _IOT_ext2_new_group_input _IOT(_IOTS(__u32), 5, _IOTS(__u16), 2, 0, 0)
#endif

#define EXT2_IOC_GETFLAGS _IOR('f', 1, long)
#define EXT2_IOC_SETFLAGS _IOW('f', 2, long)
#define EXT2_IOC_GETVERSION _IOR('v', 1, long)
#define EXT2_IOC_SETVERSION _IOW('v', 2, long)
#define EXT2_IOC_GETVERSION_NEW _IOR('f', 3, long)
#define EXT2_IOC_SETVERSION_NEW _IOW('f', 4, long)
#define EXT2_IOC_GROUP_EXTEND _IOW('f', 7, unsigned long)
#define EXT2_IOC_GROUP_ADD _IOW('f', 8, struct ext2_new_group_input)
#define EXT4_IOC_GROUP_ADD _IOW('f', 8, struct ext4_new_group_input)

struct ext2_inode {
    __u16 i_mode;         
    __u16 i_uid;          
    __u32 i_size;         
    __u32 i_atime;        
    __u32 i_ctime;        
    __u32 i_mtime;        
    __u32 i_dtime;        
    __u16 i_gid;          
    __u16 i_links_count;  
    __u32 i_blocks;       
    __u32 i_flags;        
    union {
        struct {
            __u32 l_i_version;  
        } linux1;
        struct {
            __u32 h_i_translator;
        } hurd1;
    } osd1;                        
    __u32 i_block[EXT2_N_BLOCKS];  
    __u32 i_generation;            
    __u32 i_file_acl;              
    __u32 i_dir_acl;               
    __u32 i_faddr;                 
    union {
        struct {
            __u16 l_i_blocks_hi;
            __u16 l_i_file_acl_high;
            __u16 l_i_uid_high; 
            __u16 l_i_gid_high; 
            __u32 l_i_reserved2;
        } linux2;
        struct {
            __u8 h_i_frag;  
            __u8 h_i_fsize; 
            __u16 h_i_mode_high;
            __u16 h_i_uid_high;
            __u16 h_i_gid_high;
            __u32 h_i_author;
        } hurd2;
    } osd2; 
};

struct ext2_inode_large {
    __u16 i_mode;        
    __u16 i_uid;          
    __u32 i_size;         
    __u32 i_atime;        
    __u32 i_ctime;        
    __u32 i_mtime;        
    __u32 i_dtime;        
    __u16 i_gid;          
    __u16 i_links_count;  
    __u32 i_blocks;       
    __u32 i_flags;        
    union {
        struct {
            __u32 l_i_version;  
        } linux1;
        struct {
            __u32 h_i_translator;
        } hurd1;
    } osd1;                        
    __u32 i_block[EXT2_N_BLOCKS];  
    __u32 i_generation;            
    __u32 i_file_acl;              
    __u32 i_dir_acl;               
    __u32 i_faddr;                 
    union {
        struct {
            __u16 l_i_blocks_hi;
            __u16 l_i_file_acl_high;
            __u16 l_i_uid_high; 
            __u16 l_i_gid_high; 
            __u32 l_i_reserved2;
        } linux2;
        struct {
            __u8 h_i_frag;  
            __u8 h_i_fsize; 
            __u16 h_i_mode_high;
            __u16 h_i_uid_high;
            __u16 h_i_gid_high;
            __u32 h_i_author;
        } hurd2;
    } osd2; 
    __u16 i_extra_isize;
    __u16 i_pad1;
    __u32 i_ctime_extra;  
    __u32 i_mtime_extra;  
    __u32 i_atime_extra;  
    __u32 i_crtime;       
    __u32 i_crtime_extra; 
    __u32 i_version_hi;   
};

#define i_size_high i_dir_acl

#if defined(__KERNEL__) || defined(__linux__)
#    define i_reserved1 osd1.linux1.l_i_reserved1
#    define i_frag osd2.linux2.l_i_frag
#    define i_fsize osd2.linux2.l_i_fsize
#    define i_uid_low i_uid
#    define i_gid_low i_gid
#    define i_uid_high osd2.linux2.l_i_uid_high
#    define i_gid_high osd2.linux2.l_i_gid_high
#    define i_reserved2 osd2.linux2.l_i_reserved2
#else
#    if defined(__GNU__)

#        define i_translator osd1.hurd1.h_i_translator
#        define i_frag osd2.hurd2.h_i_frag;
#        define i_fsize osd2.hurd2.h_i_fsize;
#        define i_uid_high osd2.hurd2.h_i_uid_high
#        define i_gid_high osd2.hurd2.h_i_gid_high
#        define i_author osd2.hurd2.h_i_author

#    endif 
#endif     

#define inode_uid(inode) ((inode).i_uid | (inode).osd2.linux2.l_i_uid_high << 16)
#define inode_gid(inode) ((inode).i_gid | (inode).osd2.linux2.l_i_gid_high << 16)
#define ext2fs_set_i_uid_high(inode, x) ((inode).osd2.linux2.l_i_uid_high = (x))
#define ext2fs_set_i_gid_high(inode, x) ((inode).osd2.linux2.l_i_gid_high = (x))

#define EXT2_VALID_FS 0x0001  
#define EXT2_ERROR_FS 0x0002  
#define EXT3_ORPHAN_FS 0x0004 


#define EXT2_FLAGS_SIGNED_HASH 0x0001   
#define EXT2_FLAGS_UNSIGNED_HASH 0x0002 
#define EXT2_FLAGS_TEST_FILESYS 0x0004  

#define EXT2_MOUNT_CHECK 0x0001        
#define EXT2_MOUNT_GRPID 0x0004        
#define EXT2_MOUNT_DEBUG 0x0008        
#define EXT2_MOUNT_ERRORS_CONT 0x0010  
#define EXT2_MOUNT_ERRORS_RO 0x0020    
#define EXT2_MOUNT_ERRORS_PANIC 0x0040 
#define EXT2_MOUNT_MINIX_DF 0x0080     
#define EXT2_MOUNT_NO_UID32 0x0200     

#define clear_opt(o, opt) o &= ~EXT2_MOUNT_##opt
#define set_opt(o, opt) o |= EXT2_MOUNT_##opt
#define test_opt(sb, opt) (EXT2_SB(sb)->s_mount_opt & EXT2_MOUNT_##opt)

#define EXT2_DFL_MAX_MNT_COUNT 20 
#define EXT2_DFL_CHECKINTERVAL 0  


#define EXT2_ERRORS_CONTINUE 1 
#define EXT2_ERRORS_RO 2       
#define EXT2_ERRORS_PANIC 3    
#define EXT2_ERRORS_DEFAULT EXT2_ERRORS_CONTINUE

struct ext2_super_block {
    __u32 s_inodes_count;     
    __u32 s_blocks_count;     
    __u32 s_r_blocks_count;   
    __u32 s_free_blocks_count;  
    __u32 s_free_inodes_count; 
    __u32 s_first_data_block;  
    __u32 s_log_block_size;    
    __s32 s_log_frag_size;     
    __u32 s_blocks_per_group;  
    __u32 s_frags_per_group;   
    __u32 s_inodes_per_group;   
    __u32 s_mtime;              
    __u32 s_wtime;              
    __u16 s_mnt_count;          
    __s16 s_max_mnt_count;     
    __u16 s_magic;             
    __u16 s_state;             
    __u16 s_errors;            
    __u16 s_minor_rev_level;   
    __u32 s_lastcheck;         
    __u32 s_checkinterval;     
    __u32 s_creator_os;        
    __u32 s_rev_level;         
    __u16 s_def_resuid;        
    __u16 s_def_resgid;        

    __u32 s_first_ino;               
    __u16 s_inode_size;              
    __u16 s_block_group_nr;         
    __u32 s_feature_compat;         
    __u32 s_feature_incompat;       
    __u32 s_feature_ro_compat;      
    __u8 s_uuid[16];                
    char s_volume_name[16];         
    char s_last_mounted[64];        
    __u32 s_algorithm_usage_bitmap; 
 
    __u8 s_prealloc_blocks;      
    __u8 s_prealloc_dir_blocks;  
    __u16 s_reserved_gdt_blocks; 

    __u8 s_journal_uuid[16];  
    __u32 s_journal_inum;     
    __u32 s_journal_dev;      
    __u32 s_last_orphan;      
    __u32 s_hash_seed[4];     
    __u8 s_def_hash_version;  
    __u8 s_jnl_backup_type;  
    __u16 s_desc_size;        
    __u32 s_default_mount_opts;
    __u32 s_first_meta_bg;       
    __u32 s_mkfs_time;           
    __u32 s_jnl_blocks[17];      
    __u32 s_blocks_count_hi;     
    __u32 s_r_blocks_count_hi;   
    __u32 s_free_blocks_hi;      
    __u16 s_min_extra_isize;    
    __u16 s_want_extra_isize;    
    __u32 s_flags;               
    __u16 s_raid_stride;         
    __u16 s_mmp_interval;       
    __u64 s_mmp_block;           
    __u32 s_raid_stripe_width;   
    __u8 s_log_groups_per_flex;  
    __u8 s_reserved_char_pad;
    __u16 s_reserved_pad;   
    __u32 s_reserved[162];  
};

#define EXT2_OS_LINUX 0
#define EXT2_OS_HURD 1
#define EXT2_OBSO_OS_MASIX 2
#define EXT2_OS_FREEBSD 3
#define EXT2_OS_LITES 4


#define EXT2_GOOD_OLD_REV 0 
#define EXT2_DYNAMIC_REV 1  

#define EXT2_CURRENT_REV EXT2_GOOD_OLD_REV
#define EXT2_MAX_SUPP_REV EXT2_DYNAMIC_REV

#define EXT2_GOOD_OLD_INODE_SIZE 128

#define EXT3_JNL_BACKUP_BLOCKS 1


#define EXT2_HAS_COMPAT_FEATURE(sb, mask) \
    (EXT2_SB(sb)->s_feature_compat & (mask))
#define EXT2_HAS_RO_COMPAT_FEATURE(sb, mask) \
    (EXT2_SB(sb)->s_feature_ro_compat & (mask))
#define EXT2_HAS_INCOMPAT_FEATURE(sb, mask) \
    (EXT2_SB(sb)->s_feature_incompat & (mask))

#define EXT2_FEATURE_COMPAT_DIR_PREALLOC 0x0001
#define EXT2_FEATURE_COMPAT_IMAGIC_INODES 0x0002
#define EXT3_FEATURE_COMPAT_HAS_JOURNAL 0x0004
#define EXT2_FEATURE_COMPAT_EXT_ATTR 0x0008
#define EXT2_FEATURE_COMPAT_RESIZE_INODE 0x0010
#define EXT2_FEATURE_COMPAT_DIR_INDEX 0x0020
#define EXT2_FEATURE_COMPAT_LAZY_BG 0x0040

#define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER 0x0001
#define EXT2_FEATURE_RO_COMPAT_LARGE_FILE 0x0002
#define EXT4_FEATURE_RO_COMPAT_HUGE_FILE 0x0008
#define EXT4_FEATURE_RO_COMPAT_GDT_CSUM 0x0010
#define EXT4_FEATURE_RO_COMPAT_DIR_NLINK 0x0020
#define EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE 0x0040

#define EXT2_FEATURE_INCOMPAT_COMPRESSION 0x0001
#define EXT2_FEATURE_INCOMPAT_FILETYPE 0x0002
#define EXT3_FEATURE_INCOMPAT_RECOVER 0x0004     
#define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV 0x0008 
#define EXT2_FEATURE_INCOMPAT_META_BG 0x0010
#define EXT3_FEATURE_INCOMPAT_EXTENTS 0x0040
#define EXT4_FEATURE_INCOMPAT_64BIT 0x0080
#define EXT4_FEATURE_INCOMPAT_MMP 0x0100
#define EXT4_FEATURE_INCOMPAT_FLEX_BG 0x0200

#define EXT2_FEATURE_COMPAT_SUPP 0
#define EXT2_FEATURE_INCOMPAT_SUPP (EXT2_FEATURE_INCOMPAT_FILETYPE)
#define EXT2_FEATURE_RO_COMPAT_SUPP (EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER | EXT2_FEATURE_RO_COMPAT_LARGE_FILE | EXT4_FEATURE_RO_COMPAT_DIR_NLINK | EXT2_FEATURE_RO_COMPAT_BTREE_DIR)

#define EXT2_DEF_RESUID 0
#define EXT2_DEF_RESGID 0

#define EXT2_DEFM_DEBUG 0x0001
#define EXT2_DEFM_BSDGROUPS 0x0002
#define EXT2_DEFM_XATTR_USER 0x0004
#define EXT2_DEFM_ACL 0x0008
#define EXT2_DEFM_UID16 0x0010
#define EXT3_DEFM_JMODE 0x0060
#define EXT3_DEFM_JMODE_DATA 0x0020
#define EXT3_DEFM_JMODE_ORDERED 0x0040
#define EXT3_DEFM_JMODE_WBACK 0x0060


#define EXT2_NAME_LEN 255

struct ext2_dir_entry {
    __u32 inode;              
    __u16 rec_len;            
    __u16 name_len;           
    char name[EXT2_NAME_LEN]; 
};

struct ext2_dir_entry_2 {
    __u32 inode;  
    __u16 rec_len; 
    __u8 name_len; 
    __u8 file_type;
    char name[EXT2_NAME_LEN]; 
};

#define EXT2_FT_UNKNOWN 0
#define EXT2_FT_REG_FILE 1
#define EXT2_FT_DIR 2
#define EXT2_FT_CHRDEV 3
#define EXT2_FT_BLKDEV 4
#define EXT2_FT_FIFO 5
#define EXT2_FT_SOCK 6
#define EXT2_FT_SYMLINK 7

#define EXT2_FT_MAX 8

#define EXT2_DIR_PAD 4
#define EXT2_DIR_ROUND (EXT2_DIR_PAD - 1)
#define EXT2_DIR_REC_LEN(name_len) (((name_len) + 8 + EXT2_DIR_ROUND) & ~EXT2_DIR_ROUND)


#define EXT2_MMP_MAGIC 0x004D4D50   
#define EXT2_MMP_CLEAN 0xFF4D4D50   
#define EXT2_MMP_FSCK_ON 0xE24D4D50 

struct mmp_struct {
    __u32 mmp_magic;
    __u32 mmp_seq;
    __u64 mmp_time;
    char mmp_nodename[64];
    char mmp_bdevname[32];
    __u16 mmp_interval;
    __u16 mmp_pad1;
    __u32 mmp_pad2;
};

#define EXT2_MMP_DEF_INTERVAL 5

#endif  