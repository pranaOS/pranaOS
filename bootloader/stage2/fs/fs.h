#pragma once


namespace bootloader {
    int (char *kernel_name, unsigned int startlba, unsigned int total_lba, unsigned int disk, unsigned int blocksize_phys);
}
