#pragma once

#include <cpu/idt.h>
#include <libkernel/list.h>
#include <ipc/signal.h>
#include <locking/semaphore.h>
#include <memory/vmm.h>
#include <proc/elf.h>
#include <stdint.h>
#include <timer/timer.h>
#include <libcore/hashmap.h>
#include <libkernel/plist.h>

#define SWAPPER_PID 0
#define INIT_PID 1

#define MAX_FD 256
#define PROCESS_TRAPPED_PAGE_FAULT 0xFFFFFFFF
#define MAX_THREADS 0x10000
#define STACK_SIZE 0x2000
#define UHEAP_SIZE 0x20000

#define VM_READ 0x00000001 
#define VM_WRITE 0x00000002
#define VM_EXEC 0x00000004
#define VM_SHARED 0x00000008

#define SIGNAL_STOPED 0x01
#define SIGNAL_CONTINUED 0x02
#define SIGNAL_TERMINATED 0x04
#define EXIT_TERMINATED 0x08