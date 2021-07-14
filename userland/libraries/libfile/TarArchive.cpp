/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <libfile/TARArchive.h>
#include <libio/File.h>
#include <libio/Streams.h>
#include <libfile/TARArchive.h>
#include <libio/File.h>
#include <libio/Streams.h>

struct PACKED TARRawBlock
{
    char name[100];     
    char mode[8];       
    char uid[8];       
    char gid[8];       
    char size[12];     
    char mtime[12];   
    char chksum[8];     
    char typeflag;      
    char linkname[100]; 
    char magic[6];      
    char version[2];    
    char uname[32];     
    char gname[32];     
    char devmajor[8];   
    char devminor[8];  
    char prefix[155];  
    

    char __padding[12];

    size_t file_size()
    {
        size_t size = 0;
        size_t count = 1;

        for (size_t j = 11; j > 0; j--, count *= 8)
        {
            size += ((this->size[j - 1] - '0') * count);
        }

        return size;
    }
};

bool tar_read(void *tarfile, TARBlock *block, size_t index)
{
    TARRawBlock *header = (TARRawBlock *)tarfile;

    for (size_t i = 0; i < index; i++)
    {
        if (header->name[0] == '\0')
        {
            return false;
        }

        size_t size = header->file_size();

        header = (TARRawBlock *)((char *)header + ((size / 512) + 1) * 512);

        if (size % 512)
        {
            header = (TARRawBlock *)((char *)header + 512);
        }
    }

    if (header->name[0] == '\0')
    {
        return false;
    }

    memcpy(block->name, header->name, 100);
    block->size = header->file_size();
    block->typeflag = header->typeflag;
    memcpy(block->linkname, header->linkname, 100);
    block->data = (char *)header + 512;

    return true;
}