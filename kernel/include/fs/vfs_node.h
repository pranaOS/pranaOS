#pragma once

#include <fs/dirent.h>
#include <fs/fs.h>
#include <sys/types.h>

struct DIR_s;
typedef struct DIR_s DIR;

struct filesystem_s;
typedef struct filesystem_s filesystem_t;

#include <stddef.h>
#include <stdint.h>

#define VFS_FILE        0x01
#define VFS_DIRECTORY   0x02
#define VFS_CHARDEVICE  0x03
#define VFS_BLOCKDEVICE 0x04
#define VFS_PIPE        0x05
#define VFS_SYMLINK     0x06

#define VFS_MOUNTPOINT 0x08
#define VFS_SOCKET     0x09

#define VFS_READ  0
#define VFS_WRITE 1

struct dirent;
typedef struct vfs_node_s vfs_node_t;

typedef ssize_t (*read_fpointer)(vfs_node_t *, unsigned int offset,
                                 void *buffer, size_t size, int flags);
typedef ssize_t (*write_fpointer)(vfs_node_t *, unsigned int offset,
                                  const void *buffer, size_t size, int flags);
typedef int (*close_fpointer)(vfs_node_t *);
typedef int (*open_fpointer)(vfs_node_t *, int already_exists, int flags,
                             int mode);
typedef int (*creat_fpointer)(vfs_node_t *, char *name, flags_t flags);

typedef struct dirent *(*read_dir_fpointer)(DIR *dirstream);
typedef DIR *(*open_dir_fpointer)(vfs_node_t *);
typedef int (*close_dir_fpointer)(DIR *dirstream);
typedef int (*ioctl_fpointer)(int request, char *args);

struct vfs_node_s
{
	char *   name; 
	uint8_t  type; 
	uint16_t permissions; 
	uid_t    uid;         
	gid_t    gid;         
	id_t     id;          
	size_t   filelength;  
	filesystem_t
		*    fs_info;
	offset_t offset; 
	nlink_t nlink;    

	open_fpointer  open;  
	close_fpointer close; 

	read_fpointer  read;  
	write_fpointer write; 
	creat_fpointer creat; 
	ioctl_fpointer cmd; 

	open_dir_fpointer  opendir;
	close_dir_fpointer closedir;
	read_dir_fpointer  readdir; 

	vfs_node_t *ptr;      
	vfs_node_t *nextnode; 
	vfs_node_t *dirlist;  
	                      
	vfs_node_t *parent;   
};