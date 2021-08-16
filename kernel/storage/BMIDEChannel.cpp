/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/Sections.h>
#include <kernel/storage/ATA.h>
#include <kernel/storage/BMIDEChannel.h>
#include <kernel/storage/IDEController.h>
#include <kernel/WorkQueue.h>

namespace Kernel {

UNMAP_AFTER_INIT NonnullRefPtr<BMIDEChannel> BMIDEChannel::create(const IDEController& ide_controller, IDEChannel::IOAddressGroup io_group, IDEChannel::ChannelType type)
{
    return adopt_ref(*new BMIDEChannel(ide_controller, io_group, type));
}

UNMAP_AFTER_INIT NonnullRefPtr<BMIDEChannel> BMIDEChannel::create(const IDEController& ide_controller, u8 irq, IDEChannel::IOAddressGroup io_group, IDEChannel::ChannelType type)
{
    return adopt_ref(*new BMIDEChannel(ide_controller, irq, io_group, type));
}

UNMAP_AFTER_INIT BMIDEChannel::BMIDEChannel(const IDEController& controller, IDEChannel::IOAddressGroup io_group, IDEChannel::ChannelType type)
    : IDEChannel(controller, io_group, type)
{
    initialize();
}

UNMAP_AFTER_INIT BMIDEChannel::BMIDEChannel(const IDEController& controller, u8 irq, IDEChannel::IOAddressGroup io_group, IDEChannel::ChannelType type)
    : IDEChannel(controller, irq, io_group, type)
{
    initialize();
}

UNMAP_AFTER_INIT void BMIDEChannel::initialize()
{
    VERIFY(m_io_group.bus_master_base().has_value());

    PCI::enable_bus_mastering(m_parent_controller->pci_address());
    m_prdt_page = MM.allocate_supervisor_physical_page();
    m_dma_buffer_page = MM.allocate_supervisor_physical_page();
    if (m_dma_buffer_page.is_null() || m_prdt_page.is_null())
        return;
    m_prdt_region = MM.allocate_kernel_region(m_prdt_page->paddr(), PAGE_SIZE, "IDE PRDT", Memory::Region::Access::ReadWrite);
    m_dma_buffer_region = MM.allocate_kernel_region(m_dma_buffer_page->paddr(), PAGE_SIZE, "IDE DMA region", Memory::Region::Access::ReadWrite);
    prdt().end_of_table = 0x8000;

    m_io_group.bus_master_base().value().offset(2).out<u8>(m_io_group.bus_master_base().value().offset(2).in<u8>() | 4);
}

static void print_ide_status(u8 status)
{
    dbgln("BMIDEChannel: print_ide_status: DRQ={} BSY={}, DRDY={}, DSC={}, DF={}, CORR={}, IDX={}, ERR={}",
        (status & ATA_SR_DRQ) != 0,
        (status & ATA_SR_BSY) != 0,
        (status & ATA_SR_DRDY) != 0,
        (status & ATA_SR_DSC) != 0,
        (status & ATA_SR_DF) != 0,
        (status & ATA_SR_CORR) != 0,
        (status & ATA_SR_IDX) != 0,
        (status & ATA_SR_ERR) != 0);
}

bool BMIDEChannel::handle_irq(const RegisterState&)
{
    u8 status = m_io_group.io_base().offset(ATA_REG_STATUS).in<u8>();

    m_entropy_source.add_random_event(status);

    VERIFY(m_io_group.bus_master_base().has_value());
    u8 bstatus = m_io_group.bus_master_base().value().offset(2).in<u8>();
    if (!(bstatus & 0x4)) {

        dbgln_if(PATA_DEBUG, "BMIDEChannel: ignore interrupt");
        return false;
    }

    m_io_group.bus_master_base().value().offset(2).out<u8>(m_io_group.bus_master_base().value().offset(2).in<u8>() | 4);

    ScopedSpinLock lock(m_request_lock);
    dbgln_if(PATA_DEBUG, "BMIDEChannel: interrupt: DRQ={}, BSY={}, DRDY={}",
        (status & ATA_SR_DRQ) != 0,
        (status & ATA_SR_BSY) != 0,
        (status & ATA_SR_DRDY) != 0);

    if (!m_current_request) {
        dbgln("BMIDEChannel: IRQ but no pending request!");
        return false;
    }

    if (status & ATA_SR_ERR) {
        print_ide_status(status);
        m_device_error = m_io_group.io_base().offset(ATA_REG_ERROR).in<u8>();
        dbgln("BMIDEChannel: Error {:#02x}!", (u8)m_device_error);
        try_disambiguate_error();
        complete_current_request(AsyncDeviceRequest::Failure);
        return true;
    }
    m_device_error = 0;
    complete_current_request(AsyncDeviceRequest::Success);
    return true;
}

void BMIDEChannel::complete_current_request(AsyncDeviceRequest::RequestResult result)
{

    VERIFY(m_current_request);
    VERIFY(m_request_lock.is_locked());

    g_io_work->queue([this, result]() {
        dbgln_if(PATA_DEBUG, "BMIDEChannel::complete_current_request result: {}", (int)result);
        ScopedSpinLock lock(m_request_lock);
        VERIFY(m_current_request);
        auto current_request = m_current_request;
        m_current_request.clear();

        if (result == AsyncDeviceRequest::Success) {
            if (current_request->request_type() == AsyncBlockDeviceRequest::Read) {
                if (!current_request->write_to_buffer(current_request->buffer(), m_dma_buffer_region->vaddr().as_ptr(), 512 * current_request->block_count())) {
                    lock.unlock();
                    current_request->complete(AsyncDeviceRequest::MemoryFault);
                    return;
                }
            }

            VERIFY(m_io_group.bus_master_base().has_value());
            m_io_group.bus_master_base().value().offset(2).out<u8>(m_io_group.bus_master_base().value().offset(2).in<u8>() | 0x6);
        }

        lock.unlock();
        current_request->complete(result);
    });
}

void BMIDEChannel::ata_write_sectors(bool slave_request, u16 capabilities)
{
    VERIFY(m_lock.is_locked());
    VERIFY(!m_current_request.is_null());
    VERIFY(m_current_request->block_count() <= 256);

    ScopedSpinLock m_lock(m_request_lock);
    dbgln_if(PATA_DEBUG, "BMIDEChannel::ata_write_sectors ({} x {})", m_current_request->block_index(), m_current_request->block_count());

    prdt().offset = m_dma_buffer_page->paddr().get();
    prdt().size = 512 * m_current_request->block_count();

    if (!m_current_request->read_from_buffer(m_current_request->buffer(), m_dma_buffer_region->vaddr().as_ptr(), 512 * m_current_request->block_count())) {
        complete_current_request(AsyncDeviceRequest::MemoryFault);
        return;
    }

    m_io_group.io_base().offset(ATA_REG_HDDEVSEL).out<u8>(0xA0 | ((slave_request ? 1 : 0) << 4));
    IO::delay(10);

    VERIFY(prdt().size <= PAGE_SIZE);
    VERIFY(m_io_group.bus_master_base().has_value());

    m_io_group.bus_master_base().value().out<u8>(0);

    m_io_group.bus_master_base().value().offset(4).out<u32>(m_prdt_page->paddr().get());

    m_io_group.bus_master_base().value().offset(2).out<u8>(m_io_group.bus_master_base().value().offset(2).in<u8>() | 0x6);

    ata_access(Direction::Write, slave_request, m_current_request->block_index(), m_current_request->block_count(), capabilities);

    m_io_group.bus_master_base().value().out<u8>(0x1);
}

void BMIDEChannel::send_ata_io_command(LBAMode lba_mode, Direction direction) const
{
    if (lba_mode != LBAMode::FortyEightBit) {
        m_io_group.io_base().offset(ATA_REG_COMMAND).out<u8>(direction == Direction::Read ? ATA_CMD_READ_DMA : ATA_CMD_WRITE_DMA);
    } else {
        m_io_group.io_base().offset(ATA_REG_COMMAND).out<u8>(direction == Direction::Read ? ATA_CMD_READ_DMA_EXT : ATA_CMD_WRITE_DMA_EXT);
    }
}

void BMIDEChannel::ata_read_sectors(bool slave_request, u16 capabilities)
{
    VERIFY(m_lock.is_locked());
    VERIFY(!m_current_request.is_null());
    VERIFY(m_current_request->block_count() <= 256);

    ScopedSpinLock m_lock(m_request_lock);
    dbgln_if(PATA_DEBUG, "BMIDEChannel::ata_read_sectors ({} x {})", m_current_request->block_index(), m_current_request->block_count());

    m_io_group.io_base().offset(ATA_REG_HDDEVSEL).out<u8>(0xA0 | ((slave_request ? 1 : 0) << 4));
    IO::delay(10);

    prdt().offset = m_dma_buffer_page->paddr().get();
    prdt().size = 512 * m_current_request->block_count();

    VERIFY(prdt().size <= PAGE_SIZE);

    VERIFY(m_io_group.bus_master_base().has_value());

    m_io_group.bus_master_base().value().out<u8>(0);

    m_io_group.bus_master_base().value().offset(4).out<u32>(m_prdt_page->paddr().get());

    m_io_group.bus_master_base().value().out<u8>(0x8);

    m_io_group.bus_master_base().value().offset(2).out<u8>(m_io_group.bus_master_base().value().offset(2).in<u8>() | 0x6);

    ata_access(Direction::Read, slave_request, m_current_request->block_index(), m_current_request->block_count(), capabilities);

    m_io_group.bus_master_base().value().out<u8>(0x9);
}

}
