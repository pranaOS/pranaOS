/**
 * @file pci.h
 * @author Krisna Pranav
 * @brief PCI
 * @version 6.0
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <kernel/kstd/kstddef.h>


#define PCI_ADDRESS_PORT 0xCF8
#define PCI_DATA_PORT 0xCFC
#define PCI_VENDOR_ID 0x0  
#define PCI_DEVICE_ID 0x2 
#define PCI_COMMAND 0x4  
#define PCI_STATUS 0x6 
#define PCI_REVISION_ID 0x8  
#define PCI_PROG_IF 0x9 
#define PCI_SUBCLASS 0xa  
#define PCI_CLASS 0xb  
#define PCI_CACHE_LINE_SIZE 0xc  
#define PCI_LATENCY_TIMER 0xd 
#define PCI_HEADER_TYPE 0xe  
#define PCI_BIST 0xf  
#define PCI_BAR0 0x10  
#define PCI_BAR1 0x14  
#define PCI_BAR2 0x18  
#define PCI_BAR3 0x1C  
#define PCI_BAR4 0x20  
#define PCI_BAR5 0x24  
#define PCI_PRIMARY_BUS 0x18  
#define PCI_SECONDARY_BUS 0x19  
#define PCI_INTERRUPT_LINE 0x3c  
#define PCI_INTERRUPT_PIN 0x3d  
#define PCI_MULTIFUNCTION 0x80
#define PCI_NONE 0xFFFF
#define PCI_UNCLASSIFIED 0x0
#define PCI_MASS_STORAGE_CONTROLLER 0x1
#define PCI_NETWORK_CONTROLLER 0x2
#define PCI_DISPLAY_CONTROLLER 0x3
#define PCI_MULTIMEDIA_CONTROLLER 0x4
#define PCI_MEMORY_CONTROLLER 0x5
#define PCI_BRIDGE_DEVICE 0x6
#define PCI_SIMPLE_COMMUNICATION_CONTROLLER 0x7
#define PCI_BASE_SYSTEM_PERIPHERAL 0x8
#define PCI_INPUT_DEVICE_CONTROLLER 0x9
#define PCI_DOCKING_STATION 0xA
#define PCI_PROCESSOR 0xB
#define PCI_SERIAL_BUS_CONTROLLER 0xC
#define PCI_WIRELESS_CONTROLLER 0xD
#define PCI_INTELLIGENT_CONTROLLER 0xE
#define PCI_SATELLITE_COMMUNICATION_CONTROLLER 0xF
#define PCI_ENCRYPTION_CONTROLLER 0x10
#define PCI_SIGNAL_PROCESSING_CONTROLLER 0x11
#define PCI_PROCESSING_ACCELERATOR 0x12
#define PCI_NON_ESSENTIAL_INSTRUMENTATION 0x13
#define PCI_COPROCESSOR 0x40
#define PCI_UNASSIGNED_CLASS 0xFF
#define PCI_PCI_BRIDGE 0x4
#define PCI_IDE_CONTROLLER 0x1
#define PCI_TYPE_BRIDGE 0x0604
#define PCI_TYPE_IDE_CONTROLLER 0x0101

namespace PCI {
	union IOAddress {
		struct __attribute((packed)) attrs {
			uint8_t field: 8;
			uint8_t function: 3;
			uint8_t slot: 5;
			uint8_t bus: 8;
			uint8_t : 7;
			bool enable: 1;
		} attrs;
		uint32_t value;
	};

	union Command {
		struct __attribute((packed)) attrs {
			bool io_space : 1;
			bool mem_space : 1;
			bool bus_master : 1;
			bool special_cycles : 1;
			bool mem_write_invalidate_enable : 1;
			bool VGA_palette_snoop : 1;
			bool parity_error_response : 1;
			bool : 1;
			bool serr_enable : 1;
			bool fast_back_to_back_enable : 1;
			bool interrupt_disable : 1;
			uint8_t reserved2 : 5;
		} attrs;
		uint16_t value;
	};

	class Address {
	public:
		uint8_t bus;
		uint8_t slot;
		uint8_t function;
		IOAddress get_io_address(uint8_t field);
		bool is_zero();
	};

	class ID {
	public:
		uint16_t vendor;
		uint16_t device;
		bool operator==(ID& other) const;
	};

    /**
     * @brief PCIEnumerationCallBack void
     * 
     */
	typedef void (*PCIEnumerationCallback)(Address addr, ID id, uint16_t type, void* dataPtr);

    /**
     * @brief read_byte
     * 
     * @param address 
     * @param field 
     * @return uint8_t 
     */
	uint8_t read_byte(Address address, uint8_t field);

    /**
     * @brief read_word
     * 
     * @param address 
     * @param field 
     * @return uint16_t 
     */
	uint16_t read_word(Address address, uint8_t field);

    /**
     * @brief read_dwords
     * 
     * @param address 
     * @param field 
     * @return uint32_t 
     */
	uint32_t read_dword(Address address, uint8_t field);

    /**
     * @brief write_byte
     * 
     * @param address 
     * @param field 
     * @param value 
     */
	void write_byte(Address address, uint8_t field, uint8_t value);

    /**
     * @brief write_word
     * 
     * @param address 
     * @param field 
     * @param value 
     */
	void write_word(Address address, uint8_t field, uint16_t value);

    /**
     * @brief write_dword
     * 
     * @param address 
     * @param field 
     * @param value 
     */
	void write_dword(Address address, uint8_t field, uint32_t value);

    /**
     * @brief enable_interrupt
     * 
     * @param address 
     */
	void enable_interrupt(Address address);

    /**
     * @brief disable_interrupt
     * 
     * @param address 
     */
	void disable_interrupt(Address address);

    /**
     * @brief enable_bus_mastering 
     * 
     * @param address 
     */
	void enable_bus_mastering(Address address);

    /**
     * @brief disable_bus_mastering
     * 
     * @param address 
     */
	void disable_bus_mastering(Address address);

    /**
     * @brief enumerate_devices
     * 
     * @param callback 
     * @param dataPtr 
     */
	void enumerate_devices(PCIEnumerationCallback callback, void* dataPtr);

    /**
     * @brief enumerate_bus
     * 
     * @param bus 
     * @param callback 
     * @param dataPtr 
     */
	void enumerate_bus(uint8_t bus, PCIEnumerationCallback callback, void* dataPtr);

    /**
     * @brief enumerate_slot
     * 
     * @param bus 
     * @param slot 
     * @param callback 
     * @param dataPtr 
     */
	void enumerate_slot(uint8_t bus, uint8_t slot, PCIEnumerationCallback callback, void* dataPtr);

    /**
     * @brief enumerate_functions
     * 
     * @param bus 
     * @param slot 
     * @param function 
     * @param callback 
     * @param dataPtr 
     */
	void enumerate_functions(uint8_t bus, uint8_t slot, uint8_t function, PCIEnumerationCallback callback, void* dataPtr);

    /**
     * @brief Get the class object
     * 
     * @param address 
     * @return uint8_t 
     */
	uint8_t get_class(Address address);

    /**
     * @brief Get the subclass object
     * 
     * @param address 
     * @return uint8_t 
     */
	uint8_t get_subclass(Address address);

    /**
     * @brief Get the type object
     * 
     * @param address 
     * @return uint16_t 
     */
	uint16_t get_type(Address address);

};

