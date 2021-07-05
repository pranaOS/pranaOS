/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include "system/Streams.h"
#include "system/scheduling/Scheduler.h"
#include "system/tasking/Task.h"
#include "ac97/AC97.h"


void AC97::initialise_buffers()
{
    buffer_descriptors_range = make<MMIORange>(sizeof(AC97BufferDescriptor) * AC97_BDL_LEN);
    buffer_descriptors_list = reinterpret_cast<AC97BufferDescriptor *>(buffer_descriptors_range->base());

    for (size_t i = 0; i < AC97_BDL_LEN; i++)
    {
        buffers.push_back(make<MMIORange>((size_t)AC97_BDL_BUFFER_LEN * 2));
        buffer_descriptors_list[i].pointer = buffers[i]->physical_base();
        AC97_CL_SET_LENGTH(buffer_descriptors_list[i].cl, AC97_BDL_BUFFER_LEN);
        memset((char *)buffers[i]->base(), 0, AC97_BDL_BUFFER_LEN * 2);

        buffer_descriptors_list[i].cl |= AC97_CL_IOC;
    }
}
