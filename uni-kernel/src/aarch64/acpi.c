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

/**
 * @param irq
 * @param t
 * @param name
 */
void acpi_register_irq_handler(int irq, thunk t, sstring name)
{
    irq_register_handle(irq, t, name, irange(0, 0));
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
    default:
        break;
    }
}