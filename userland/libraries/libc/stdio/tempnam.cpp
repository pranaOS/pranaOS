/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define TEMP_DIR "/Temp/"

char *tmpnam(const char *pfx)
{
    return tempnam(NULL, pfx);
}

char *tempnam(const char *dir, const char *pfx)
{
    int plen;
    if (pfx == NULL || !pfx[0])
    {
        pfx = "file";
        plen = 4;
    }
    else
    {
        plen = strlen(pfx);
        if (plen > 5)
        {
            plen = 5;
        }
    }

    if (dir == NULL)
    {
        dir = TEMP_DIR;
    }
    else
    {
        assert(false);
    }


    static unsigned int num = 0;
    char num_buf[6];
    snprintf(num_buf, 6, "%u", num);
    num++;

    char out_path[256];

    strcat(out_path, TEMP_DIR);
    strcat(out_path, pfx);
    strcat(out_path, num_buf);

    return strdup(out_path);
}