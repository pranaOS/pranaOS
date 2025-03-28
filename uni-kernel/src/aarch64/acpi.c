/**
 * @file acpi.c
 * @author Krisna Pranav
 * @brief acpi
 * @version 6.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <acpi.h>
#include <boot/uefi.h>
#include <drivers/acpi.h>
#include <kernel.h>

/**
 * @param irq
 * @param t
 * @param name
 */
void acpi_register_irq_handler(int irq, thunk t, sstring name)
{
    irq_register_handler(irq, t, name, irange(0, 0));
}

ACPI_PHYSICAL_ADDRESS AcpiOsGetRootPointer(void)
{
    return boot_params.acpi_rsdp;
}

/**
 * @param address
 * @param value
 * @param width
 * @return ACPI_STATUS
 */
ACPI_STATUS AcpiOsReadPort(ACPI_IO_ADDRESS address, UINT32* value, UINT32 width)
{
    switch(width)
    {
    case 8:
        *value = mmio_read_8(address);
        break;
    case 16:
        *value = mmio_read_16(address);
        break;
    case 32:
        *value = mmio_read_32(address);
        break;
    default:
        return AE_BAD_PARAMETER;
    }
    return AE_OK;
}

/**
 * @param address
 * @param value
 * @param width
 * @return ACPI_STATUS
 */
ACPI_STATUS AcpiOsWritePort(ACPI_IO_ADDRESS address, UINT32 value, UINT32 width)
{
    switch(width)
    {
    case 8:
        mmio_write_8(address, value);
        break;
    case 16:
        mmio_write_16(address, value);
        break;
    case 32:
        mmio_write_32(address, value);
        break;
    default:
        return AE_BAD_PARAMETER;
    }
    return AE_OK;
}