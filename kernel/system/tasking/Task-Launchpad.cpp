/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <libfile/ELF.h>
#include <libsystem/io/Stream.h>
#include <string.h>
#include "system/Streams.h"
#include "system/interrupts/Interupts.h"
#include "system/scheduling/Scheduler.h"
#include "system/tasking/Task-Launchpad.h"
#include "system/tasking/Task-Memory.h"
#include "system/tasking/Task.h"

template <typename TELFFormat>
struct ElFLoader
{
    using Header = TELFFormat::Header;
    using Section = TELFFormat::Section;
    using Program = TELFFormat::Program;
    using Symbols = TELFFormat::Symbols;

    static JResult load_program(Task *task, Stream *elf_file, Program *program_header)
    {
        if (program_header->vaddr == 0)
        {
            return SUCCESS;
        }

        if (program_header->vaddr <= 0x100000)
        {
            Kernel::logln("ELF program no in user memory ({p})!", program_header->vaddr);
            return ERR_EXEC_FORMAT_ERROR;
        }

        auto *parent_address_space = task_switch_address_space(scheduler_running(), task->address_space);

        MemoryRange range = MemoryRange::around_non_aligned_address(program_header->vaddr, program_header->memsz);

        task_memory_map(task, range.base(), range.size(), MEMORY_CLEAR);

        stream_seek(elf_file, IO::SeekFrom::start(program_header->offset));
        size_t read = stream_read(elf_file, (void *)program_header->vaddr, program_header->filesz);

        if (read != program_header->filesz)
        {
            Kernel::logln("Didn't read the right amount from the ELF file!");

            task_switch_address_space(scheduler_running(), parent_address_space);

            return ERR_EXEC_FORMAT_ERROR;
        }
        else
        {
            task_switch_address_space(scheduler_running(), parent_address_space);

            return SUCCESS;
        }
    }

};