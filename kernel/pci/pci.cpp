/**
* @file pci.cpp
* @author Krisna Pranav
* @brief PCI
* @version 1.0
* @date 2023-06-19
*
* @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
*
*/

#include "pci.h"
#include "../io.h"

namespace PCI {
    uint8_t read_byte(Address address, uint8_t field) {
        IO::outl(PCI_ADDRESS_PORT, address.get_io_address(field).value);
        return IO::inb(PCI_DATA_PORT + (field & 3));
    }

    uint16_t read_word(Address address, uint8_t field) {
        IO::outl(PCI_ADDRESS_PORT, address.get_io_address(field).value);
        return IO::inw(PCI_DATA_PORT + (field & 2));
    }

    uint32_t read_dword(Address address, uint8_t field) {
        IO::outl(PCI_ADDRESS_PORT, address.get_io_address(field).value);
        return IO::inl(PCI_DATA_PORT);
    }

    void write_byte(Address address, uint8_t field, uint8_t value) {
        IO::outl(PCI_ADDRESS_PORT, address.get_io_address(field).value);
        IO::outb(PCI_DATA_PORT + (field & 3), value);
    }

    void write_word(Address address, uint8_t field, uint16_t value) {
        IO::outl(PCI_ADDRESS_PORT, address.get_io_address(field).value);
        IO::outw(PCI_DATA_PORT + (field & 2), value);
    }

    void write_dword(Address address, uint8_t field, uint32_t value) {
        IO::outl(PCI_ADDRESS_PORT, address.get_io_address(field).value);
        IO::outl(PCI_DATA_PORT, value);
    }

}