//
// Created by KrisnaPranav on 22/01/22.
//

/**
 * @brief main kernel entry point 
 * 
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "cpu/exception.h"
#include "cpu/gdtentry.h"
#include "cpu/halt.h"
#include "cpu/idt.h"
#include "cpu/rtc.h"
#include "cpu/tasksegment.h"
#include "devices/ata.h"
#include "devices/memory/memory.h"
#include "filesystem/vfs.h"

extern struct vfs_file_system_type ext2_file_type;

// TODO: setup window server[while booting]
void setup_server() {
}
