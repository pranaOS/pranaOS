#pragma once

#include <libc/__libc__.h>
#include <libabi/result.h>
#include <libutils/prelude.h>
#include <libabi/handle.h>
#include <libabi/iocall.h>
#include <libabi/launchpad.h>
#include <libabi/system.h>

#define SYSCALL_ENUM_ENTRY(__entry) __entry,

enum Syscall
{
    SYSCALL_LIST(SYSCALL_ENUM_ENTRY) __SYSCALL_COUNT
};

__BEGIN_HEADER 

JResult j_process_this(int *pid);
JResult j_process_name(char *name, size_t size);

JResult j_handle_open(int *handle, const char *raw_path, size_t size, JOpenFlag flags);
JResult j_handle_close(int handle);
JResult j_handle_repoen(int handle, int *reopened);
JResult j_handle_copy(int source, int destination);
JResult j_handle_poll(HandlePoll *handles, size_t count, Timeout timeout);
JResult j_handle_read(int handle, void *buffer, size_t size, size_t *written);
JResult j_handle_write(int handle, IOCall request, void *args);


__END_HEADER