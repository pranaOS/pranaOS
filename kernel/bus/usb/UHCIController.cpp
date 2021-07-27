/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/JsonArraySerializer.h>
#include <base/JsonObjectSerializer.h>
#include <base/Platform.h>
#include <kernel/bus/usb/UHCIController.h>
#include <kernel/bus/usb/USBRequest.h>
#include <kernel/CommandLine.h>
#include <kernel/Debug.h>
#include <kernel/filesystem/SysFS.h>
#include <kernel/KBufferBuilder.h>
#include <kernel/Process.h>
#include <kernel/Sections.h>
#include <kernel/StdLib.h>
#include <kernel/time/TimeManagement.h>
#include <kernel/vm/AnonymousVMObject.h>
#include <kernel/vm/MemoryManager.h>

static constexpr u8 MAXIMUM_NUMBER_OF_TDS = 128; 
static constexpr u8 MAXIMUM_NUMBER_OF_QHS = 64;
static constexpr u8 RETRY_COUNTER_RELOAD = 3;

namespace Kernel::USB {

static UHCIController* s_the;

static constexpr u16 UHCI_USBCMD_RUN = 0x0001;
static constexpr u16 UHCI_USBCMD_HOST_CONTROLLER_RESET = 0x0002;
static constexpr u16 UHCI_USBCMD_GLOBAL_RESET = 0x0004;
static constexpr u16 UHCI_USBCMD_ENTER_GLOBAL_SUSPEND_MODE = 0x0008;
static constexpr u16 UHCI_USBCMD_FORCE_GLOBAL_RESUME = 0x0010;
static constexpr u16 UHCI_USBCMD_SOFTWARE_DEBUG = 0x0020;
static constexpr u16 UHCI_USBCMD_CONFIGURE_FLAG = 0x0040;
static constexpr u16 UHCI_USBCMD_MAX_PACKET = 0x0080;

static constexpr u16 UHCI_USBSTS_HOST_CONTROLLER_HALTED = 0x0020;
static constexpr u16 UHCI_USBSTS_HOST_CONTROLLER_PROCESS_ERROR = 0x0010;
static constexpr u16 UHCI_USBSTS_PCI_BUS_ERROR = 0x0008;
static constexpr u16 UHCI_USBSTS_RESUME_RECEIVED = 0x0004;
static constexpr u16 UHCI_USBSTS_USB_ERROR_INTERRUPT = 0x0002;
static constexpr u16 UHCI_USBSTS_USB_INTERRUPT = 0x0001;

static constexpr u8 UHCI_USBINTR_TIMEOUT_CRC_ENABLE = 0x01;
static constexpr u8 UHCI_USBINTR_RESUME_INTR_ENABLE = 0x02;
static constexpr u8 UHCI_USBINTR_IOC_ENABLE = 0x04;
static constexpr u8 UHCI_USBINTR_SHORT_PACKET_INTR_ENABLE = 0x08;

static constexpr u16 UHCI_FRAMELIST_FRAME_COUNT = 1024; 
static constexpr u16 UHCI_FRAMELIST_FRAME_INVALID = 0x0001;

static constexpr u8 UHCI_ROOT_PORT_COUNT = 2;
static constexpr u16 UHCI_PORTSC_CURRRENT_CONNECT_STATUS = 0x0001;
static constexpr u16 UHCI_PORTSC_CONNECT_STATUS_CHANGED = 0x0002;
static constexpr u16 UHCI_PORTSC_PORT_ENABLED = 0x0004;
static constexpr u16 UHCI_PORTSC_PORT_ENABLE_CHANGED = 0x0008;
static constexpr u16 UHCI_PORTSC_LINE_STATUS = 0x0030;
static constexpr u16 UHCI_PORTSC_RESUME_DETECT = 0x40;
static constexpr u16 UHCI_PORTSC_LOW_SPEED_DEVICE = 0x0100;
static constexpr u16 UHCI_PORTSC_PORT_RESET = 0x0200;
static constexpr u16 UHCI_PORTSC_SUSPEND = 0x1000;

static constexpr u8 UHCI_NUMBER_OF_ISOCHRONOUS_TDS = 128;
static constexpr u16 UHCI_NUMBER_OF_FRAMES = 1024;

class SysFSUSBBusDirectory;
static SysFSUSBBusDirectory* s_procfs_usb_bus_directory;

class SysFSUSBDeviceInformation : public SysFSComponent {
    friend class SysFSUSBBusDirectory;

public:
    virtual ~SysFSUSBDeviceInformation() override {};

    static NonnullRefPtr<SysFSUSBDeviceInformation> create(USB::Device&);

    RefPtr<USB::Device> device() const { return m_device; }

protected:
    explicit SysFSUSBDeviceInformation(USB::Device& device)
        : SysFSComponent(String::number(device.address()))
        , m_device(device)
    {
    }

    virtual KResultOr<size_t> read_bytes(off_t offset, size_t count, UserOrKernelBuffer& buffer, FileDescription*) const override
    {
        KBufferBuilder builder;
        JsonArraySerializer array { builder };

        auto obj = array.add_object();
        obj.add("usb_spec_compliance_bcd", m_device->device_descriptor().usb_spec_compliance_bcd);
        obj.add("device_class", m_device->device_descriptor().device_class);
        obj.add("device_sub_class", m_device->device_descriptor().device_sub_class);
        obj.add("device_protocol", m_device->device_descriptor().device_protocol);
        obj.add("max_packet_size", m_device->device_descriptor().max_packet_size);
        obj.add("vendor_id", m_device->device_descriptor().vendor_id);
        obj.add("product_id", m_device->device_descriptor().product_id);
        obj.add("device_release_bcd", m_device->device_descriptor().device_release_bcd);
        obj.add("manufacturer_id_descriptor_index", m_device->device_descriptor().manufacturer_id_descriptor_index);
        obj.add("product_string_descriptor_index", m_device->device_descriptor().product_string_descriptor_index);
        obj.add("serial_number_descriptor_index", m_device->device_descriptor().serial_number_descriptor_index);
        obj.add("num_configurations", m_device->device_descriptor().num_configurations);
        obj.finish();
        array.finish();

        auto data = builder.build();
        if (!data)
            return ENOMEM;

        ssize_t nread = min(static_cast<off_t>(data->size() - offset), static_cast<off_t>(count));
        if (!buffer.write(data->data() + offset, nread))
            return EFAULT;

        return nread;
    }
    IntrusiveListNode<SysFSUSBDeviceInformation, RefPtr<SysFSUSBDeviceInformation>> m_list_node;

    NonnullRefPtr<USB::Device> m_device;
};

class SysFSUSBBusDirectory final : public SysFSDirectory {
public:
    static void initialize();
    void plug(USB::Device&);
    void unplug(USB::Device&);

    virtual KResult traverse_as_directory(unsigned, Function<bool(FileSystem::DirectoryEntryView const&)>) const override;
    virtual RefPtr<SysFSComponent> lookup(StringView name) override;

private:
    explicit SysFSUSBBusDirectory(SysFSBusDirectory&);

    RefPtr<SysFSUSBDeviceInformation> device_node_for(USB::Device& device);

    IntrusiveList<SysFSUSBDeviceInformation, RefPtr<SysFSUSBDeviceInformation>, &SysFSUSBDeviceInformation::m_list_node> m_device_nodes;
    mutable SpinLock<u8> m_lock;
};

KResult SysFSUSBBusDirectory::traverse_as_directory(unsigned fsid, Function<bool(FileSystem::DirectoryEntryView const&)> callback) const
{
    ScopedSpinLock lock(m_lock);
    VERIFY(m_parent_directory);
    callback({ ".", { fsid, component_index() }, 0 });
    callback({ "..", { fsid, m_parent_directory->component_index() }, 0 });

    for (auto& device_node : m_device_nodes) {
        InodeIdentifier identifier = { fsid, device_node.component_index() };
        callback({ device_node.name(), identifier, 0 });
    }
    return KSuccess;
}
RefPtr<SysFSComponent> SysFSUSBBusDirectory::lookup(StringView name)
{
    ScopedSpinLock lock(m_lock);
    for (auto& device_node : m_device_nodes) {
        if (device_node.name() == name) {
            return device_node;
        }
    }
    return {};
}

RefPtr<SysFSUSBDeviceInformation> SysFSUSBBusDirectory::device_node_for(USB::Device& device)
{
    RefPtr<USB::Device> checked_device = device;
    for (auto& device_node : m_device_nodes) {
        if (device_node.device().ptr() == checked_device.ptr())
            return device_node;
    }
    return {};
}

void SysFSUSBBusDirectory::plug(USB::Device& new_device)
{
    ScopedSpinLock lock(m_lock);
    auto device_node = device_node_for(new_device);
    VERIFY(!device_node);
    m_device_nodes.append(SysFSUSBDeviceInformation::create(new_device));
}

void SysFSUSBBusDirectory::unplug(USB::Device& deleted_device)
{
    ScopedSpinLock lock(m_lock);
    auto device_node = device_node_for(deleted_device);
    VERIFY(device_node);
    device_node->m_list_node.remove();
}

UNMAP_AFTER_INIT SysFSUSBBusDirectory::SysFSUSBBusDirectory(SysFSBusDirectory& buses_directory)
    : SysFSDirectory("usb"sv, buses_directory)
{
}

UNMAP_AFTER_INIT void SysFSUSBBusDirectory::initialize()
{
    auto directory = adopt_ref(*new SysFSUSBBusDirectory(SysFSComponentRegistry::the().buses_directory()));
    SysFSComponentRegistry::the().register_new_bus_directory(directory);
    s_procfs_usb_bus_directory = directory;
}

NonnullRefPtr<SysFSUSBDeviceInformation> SysFSUSBDeviceInformation::create(USB::Device& device)
{
    return adopt_ref(*new SysFSUSBDeviceInformation(device));
}

UHCIController& UHCIController::the()
{
    return *s_the;
}

UNMAP_AFTER_INIT void UHCIController::detect()
{
    if (kernel_command_line().disable_uhci_controller())
        return;

    SysFSUSBBusDirectory::initialize();

    PCI::enumerate([&](const PCI::Address& address, PCI::ID id) {
        if (address.is_null())
            return;

        if (PCI::get_class(address) == 0xc && PCI::get_subclass(address) == 0x03 && PCI::get_programming_interface(address) == 0) {
            if (!s_the) {
                s_the = new UHCIController(address, id);
                s_the->spawn_port_proc();
            }
        }
    });
}

UNMAP_AFTER_INIT UHCIController::UHCIController(PCI::Address address, PCI::ID id)
    : PCI::Device(address)
    , m_io_base(PCI::get_BAR4(pci_address()) & ~1)
{
    dmesgln("UHCI: Controller found {} @ {}", id, address);
    dmesgln("UHCI: I/O base {}", m_io_base);
    dmesgln("UHCI: Interrupt line: {}", PCI::get_interrupt_line(pci_address()));

    reset();
    start();
}

UNMAP_AFTER_INIT UHCIController::~UHCIController()
{
}

RefPtr<USB::Device> const UHCIController::get_device_at_port(USB::Device::PortNumber port)
{
    if (!m_devices.at(to_underlying(port)))
        return nullptr;

    return m_devices.at(to_underlying(port));
}

RefPtr<USB::Device> const UHCIController::get_device_from_address(u8 device_address)
{
    for (auto const& device : m_devices) {
        if (!device)
            continue;

        if (device->address() == device_address)
            return device;
    }

    return nullptr;
}

void UHCIController::reset()
{
    stop();

    write_usbcmd(UHCI_USBCMD_HOST_CONTROLLER_RESET);

    for (;;) {
        if (read_usbcmd() & UHCI_USBCMD_HOST_CONTROLLER_RESET)
            continue;
        break;
    }

    auto framelist_vmobj = AnonymousVMObject::try_create_physically_contiguous_with_size(PAGE_SIZE);
    m_framelist = MemoryManager::the().allocate_kernel_region_with_vmobject(*framelist_vmobj, PAGE_SIZE, "UHCI Framelist", Region::Access::Write);
    dbgln("UHCI: Allocated framelist at physical address {}", m_framelist->physical_page(0)->paddr());
    dbgln("UHCI: Framelist is at virtual address {}", m_framelist->vaddr());
    write_sofmod(64); 

    create_structures();
    setup_schedule();

    write_flbaseadd(m_framelist->physical_page(0)->paddr().get()); 
    write_frnum(0);                                                

    write_usbintr(0);
    dbgln("UHCI: Reset completed");
}

UNMAP_AFTER_INIT void UHCIController::create_structures()
{
    auto qh_pool_vmobject = AnonymousVMObject::try_create_physically_contiguous_with_size(2 * PAGE_SIZE);
    m_qh_pool = MemoryManager::the().allocate_kernel_region_with_vmobject(*qh_pool_vmobject, 2 * PAGE_SIZE, "UHCI Queue Head Pool", Region::Access::Write);
    memset(m_qh_pool->vaddr().as_ptr(), 0, 2 * PAGE_SIZE); 

    m_free_qh_pool.resize(MAXIMUM_NUMBER_OF_TDS);
    for (size_t i = 0; i < m_free_qh_pool.size(); i++) {
        auto placement_addr = reinterpret_cast<void*>(m_qh_pool->vaddr().get() + (i * sizeof(QueueHead)));
        auto paddr = static_cast<u32>(m_qh_pool->physical_page(0)->paddr().get() + (i * sizeof(QueueHead)));
        m_free_qh_pool.at(i) = new (placement_addr) QueueHead(paddr);
    }

    m_interrupt_transfer_queue = allocate_queue_head();
    m_lowspeed_control_qh = allocate_queue_head();
    m_fullspeed_control_qh = allocate_queue_head();
    m_bulk_qh = allocate_queue_head();
    m_dummy_qh = allocate_queue_head();

    auto td_pool_vmobject = AnonymousVMObject::try_create_physically_contiguous_with_size(2 * PAGE_SIZE);
    m_td_pool = MemoryManager::the().allocate_kernel_region_with_vmobject(*td_pool_vmobject, 2 * PAGE_SIZE, "UHCI Transfer Descriptor Pool", Region::Access::Write);
    memset(m_td_pool->vaddr().as_ptr(), 0, 2 * PAGE_SIZE);

    m_iso_td_list.resize(UHCI_NUMBER_OF_ISOCHRONOUS_TDS);
    for (size_t i = 0; i < m_iso_td_list.size(); i++) {
        auto placement_addr = reinterpret_cast<void*>(m_td_pool->vaddr().get() + (i * sizeof(Kernel::USB::TransferDescriptor)));
        auto paddr = static_cast<u32>(m_td_pool->physical_page(0)->paddr().get() + (i * sizeof(Kernel::USB::TransferDescriptor)));

        m_iso_td_list.at(i) = new (placement_addr) Kernel::USB::TransferDescriptor(paddr);
        auto transfer_descriptor = m_iso_td_list.at(i);
        transfer_descriptor->set_in_use(true);
        transfer_descriptor->set_isochronous();
        transfer_descriptor->link_queue_head(m_interrupt_transfer_queue->paddr());

        if constexpr (UHCI_VERBOSE_DEBUG)
            transfer_descriptor->print();
    }

    m_free_td_pool.resize(MAXIMUM_NUMBER_OF_TDS);
    for (size_t i = 0; i < m_free_td_pool.size(); i++) {
        auto placement_addr = reinterpret_cast<void*>(m_td_pool->vaddr().offset(PAGE_SIZE).get() + (i * sizeof(Kernel::USB::TransferDescriptor)));
        auto paddr = static_cast<u32>(m_td_pool->physical_page(1)->paddr().get() + (i * sizeof(Kernel::USB::TransferDescriptor)));

        m_free_td_pool.at(i) = new (placement_addr) Kernel::USB::TransferDescriptor(paddr);

        if constexpr (UHCI_VERBOSE_DEBUG) {
            auto transfer_descriptor = m_free_td_pool.at(i);
            transfer_descriptor->print();
        }
    }

    if constexpr (UHCI_DEBUG) {
        dbgln("UHCI: Pool information:");
        dbgln("    qh_pool: {}, length: {}", PhysicalAddress(m_qh_pool->physical_page(0)->paddr()), m_qh_pool->range().size());
        dbgln("    td_pool: {}, length: {}", PhysicalAddress(m_td_pool->physical_page(0)->paddr()), m_td_pool->range().size());
    }
}

UNMAP_AFTER_INIT void UHCIController::setup_schedule()
{
    m_interrupt_transfer_queue->link_next_queue_head(m_lowspeed_control_qh);
    m_interrupt_transfer_queue->terminate_element_link_ptr();

    m_lowspeed_control_qh->link_next_queue_head(m_fullspeed_control_qh);
    m_lowspeed_control_qh->terminate_element_link_ptr();

    m_fullspeed_control_qh->link_next_queue_head(m_bulk_qh);
    m_fullspeed_control_qh->terminate_element_link_ptr();

    m_bulk_qh->link_next_queue_head(m_dummy_qh);
    m_bulk_qh->terminate_element_link_ptr();

    auto piix4_td_hack = allocate_transfer_descriptor();
    piix4_td_hack->terminate();
    piix4_td_hack->set_max_len(0x7ff); 
    piix4_td_hack->set_device_address(0x7f);
    piix4_td_hack->set_packet_id(PacketID::IN);
    m_dummy_qh->terminate_with_stray_descriptor(piix4_td_hack);
    m_dummy_qh->terminate_element_link_ptr();

    u32* framelist = reinterpret_cast<u32*>(m_framelist->vaddr().as_ptr());
    for (int frame = 0; frame < UHCI_NUMBER_OF_FRAMES; frame++) {

        framelist[frame] = m_iso_td_list.at(frame % UHCI_NUMBER_OF_ISOCHRONOUS_TDS)->paddr();
    }

    m_interrupt_transfer_queue->print();
    m_lowspeed_control_qh->print();
    m_fullspeed_control_qh->print();
    m_bulk_qh->print();
    m_dummy_qh->print();
}

QueueHead* UHCIController::allocate_queue_head() const
{
    for (QueueHead* queue_head : m_free_qh_pool) {
        if (!queue_head->in_use()) {
            queue_head->set_in_use(true);
            dbgln_if(UHCI_DEBUG, "UHCI: Allocated a new Queue Head! Located @ {} ({})", VirtualAddress(queue_head), PhysicalAddress(queue_head->paddr()));
            return queue_head;
        }
    }

    return nullptr; 
}

TransferDescriptor* UHCIController::allocate_transfer_descriptor() const
{
    for (TransferDescriptor* transfer_descriptor : m_free_td_pool) {
        if (!transfer_descriptor->in_use()) {
            transfer_descriptor->set_in_use(true);
            dbgln_if(UHCI_DEBUG, "UHCI: Allocated a new Transfer Descriptor! Located @ {} ({})", VirtualAddress(transfer_descriptor), PhysicalAddress(transfer_descriptor->paddr()));
            return transfer_descriptor;
        }
    }

    return nullptr; 
}

void UHCIController::stop()
{
    write_usbcmd(read_usbcmd() & ~UHCI_USBCMD_RUN);
    for (;;) {
        if (read_usbsts() & UHCI_USBSTS_HOST_CONTROLLER_HALTED)
            break;
    }
}

void UHCIController::start()
{
    write_usbcmd(read_usbcmd() | UHCI_USBCMD_RUN);
    for (;;) {
        if (!(read_usbsts() & UHCI_USBSTS_HOST_CONTROLLER_HALTED))
            break;
    }
    dbgln("UHCI: Started");
}

TransferDescriptor* UHCIController::create_transfer_descriptor(Pipe& pipe, PacketID direction, size_t data_len)
{
    TransferDescriptor* td = allocate_transfer_descriptor();
    if (td == nullptr) {
        return nullptr;
    }

    u16 max_len = (data_len > 0) ? (data_len - 1) : 0x7ff;
    VERIFY(max_len <= 0x4FF || max_len == 0x7FF); 

    td->set_token((max_len << TD_TOKEN_MAXLEN_SHIFT) | ((pipe.data_toggle() ? 1 : 0) << TD_TOKEN_DATA_TOGGLE_SHIFT) | (pipe.endpoint_address() << TD_TOKEN_ENDPOINT_SHIFT) | (pipe.device_address() << TD_TOKEN_DEVICE_ADDR_SHIFT) | (static_cast<u8>(direction)));
    pipe.set_toggle(!pipe.data_toggle());

    if (pipe.type() == Pipe::Type::Isochronous) {
        td->set_isochronous();
    } else {
        if (direction == PacketID::IN) {
            td->set_short_packet_detect();
        }
    }

    if (pipe.device_speed() == Pipe::DeviceSpeed::LowSpeed) {
        td->set_lowspeed();
    }

    td->set_active();
    td->set_error_retry_counter(RETRY_COUNTER_RELOAD);

    return td;
}

KResult UHCIController::create_chain(Pipe& pipe, PacketID direction, Ptr32<u8>& buffer_address, size_t max_size, size_t transfer_size, TransferDescriptor** td_chain, TransferDescriptor** last_td)
{
    size_t byte_count = 0;
    TransferDescriptor* current_td = nullptr;
    TransferDescriptor* prev_td = nullptr;
    TransferDescriptor* first_td = nullptr;

    while (byte_count < transfer_size) {
        size_t packet_size = transfer_size - byte_count;
        if (packet_size > max_size) {
            packet_size = max_size;
        }

        current_td = create_transfer_descriptor(pipe, direction, packet_size);
        if (current_td == nullptr) {
            free_descriptor_chain(first_td);
            return ENOMEM;
        }

        if (Checked<FlatPtr>::addition_would_overflow(reinterpret_cast<FlatPtr>(&*buffer_address), byte_count))
            return EOVERFLOW;

        auto buffer_pointer = Ptr32<u8>(buffer_address + byte_count);
        current_td->set_buffer_address(buffer_pointer);
        byte_count += packet_size;

        if (prev_td != nullptr)
            prev_td->insert_next_transfer_descriptor(current_td);
        else
            first_td = current_td;

        prev_td = current_td;
    }

    *last_td = current_td;
    *td_chain = first_td;
    return KSuccess;
}

void UHCIController::free_descriptor_chain(TransferDescriptor* first_descriptor)
{
    TransferDescriptor* descriptor = first_descriptor;

    while (descriptor) {
        descriptor->free();
        descriptor = descriptor->next_td();
    }
}

KResultOr<size_t> UHCIController::submit_control_transfer(Transfer& transfer)
{
    Pipe& pipe = transfer.pipe();
    bool direction_in = (transfer.request().request_type & USB_DEVICE_REQUEST_DEVICE_TO_HOST) == USB_DEVICE_REQUEST_DEVICE_TO_HOST;

    TransferDescriptor* setup_td = create_transfer_descriptor(pipe, PacketID::SETUP, sizeof(USBRequestData));
    if (!setup_td)
        return ENOMEM;

    setup_td->set_buffer_address(transfer.buffer_physical().as_ptr());

    TransferDescriptor* last_data_descriptor;
    TransferDescriptor* data_descriptor_chain;
    auto buffer_address = Ptr32<u8>(transfer.buffer_physical().as_ptr() + sizeof(USBRequestData));
    auto transfer_chain_create_result = create_chain(pipe,
        direction_in ? PacketID::IN : PacketID::OUT,
        buffer_address,
        pipe.max_packet_size(),
        transfer.transfer_data_size(),
        &data_descriptor_chain,
        &last_data_descriptor);

    if (transfer_chain_create_result != KSuccess)
        return transfer_chain_create_result;

    pipe.set_toggle(true);

    TransferDescriptor* status_td = create_transfer_descriptor(pipe, direction_in ? PacketID::OUT : PacketID::IN, 0);
    if (!status_td) {
        free_descriptor_chain(data_descriptor_chain);
        return ENOMEM;
    }
    status_td->terminate();

    if (data_descriptor_chain) {
        setup_td->insert_next_transfer_descriptor(data_descriptor_chain);
        last_data_descriptor->insert_next_transfer_descriptor(status_td);
    } else {
        setup_td->insert_next_transfer_descriptor(status_td);
    }

    if constexpr (UHCI_VERBOSE_DEBUG) {
        dbgln("Setup TD");
        setup_td->print();
        if (data_descriptor_chain) {
            dbgln("Data TD");
            data_descriptor_chain->print();
        }
        dbgln("Status TD");
        status_td->print();
    }

    QueueHead* transfer_queue = allocate_queue_head();
    if (!transfer_queue) {
        free_descriptor_chain(data_descriptor_chain);
        return 0;
    }

    transfer_queue->attach_transfer_descriptor_chain(setup_td);
    transfer_queue->set_transfer(&transfer);

    m_fullspeed_control_qh->attach_transfer_queue(*transfer_queue);

    size_t transfer_size = 0;
    while (!transfer.complete())
        transfer_size = poll_transfer_queue(*transfer_queue);

    free_descriptor_chain(transfer_queue->get_first_td());
    transfer_queue->free();

    return transfer_size;
}

size_t UHCIController::poll_transfer_queue(QueueHead& transfer_queue)
{
    Transfer* transfer = transfer_queue.transfer();
    TransferDescriptor* descriptor = transfer_queue.get_first_td();
    bool transfer_still_in_progress = false;
    size_t transfer_size = 0;

    while (descriptor) {
        u32 status = descriptor->status();

        if (status & TransferDescriptor::StatusBits::Active) {
            transfer_still_in_progress = true;
            break;
        }

        if (status & TransferDescriptor::StatusBits::ErrorMask) {
            transfer->set_complete();
            transfer->set_error_occurred();
            dbgln_if(UHCI_DEBUG, "UHCIController: Transfer failed! Reason: {:08x}", status);
            return 0;
        }

        transfer_size += descriptor->actual_packet_length();
        descriptor = descriptor->next_td();
    }

    if (!transfer_still_in_progress)
        transfer->set_complete();

    return transfer_size;
}

void UHCIController::spawn_port_proc()
{
    RefPtr<Thread> usb_hotplug_thread;

    Process::create_kernel_process(usb_hotplug_thread, "UHCIHotplug", [&] {
        for (;;) {
            for (int port = 0; port < UHCI_ROOT_PORT_COUNT; port++) {
                u16 port_data = 0;

                if (port == 1) {
                    port_data = read_portsc1();
                    if (port_data & UHCI_PORTSC_CONNECT_STATUS_CHANGED) {
                        if (port_data & UHCI_PORTSC_CURRRENT_CONNECT_STATUS) {
                            dmesgln("UHCI: Device attach detected on Root Port 1!");

                            port_data = read_portsc1();
                            write_portsc1(port_data | UHCI_PORTSC_PORT_RESET);
                            IO::delay(500);

                            write_portsc1(port_data & ~UHCI_PORTSC_PORT_RESET);
                            IO::delay(500);

                            write_portsc1(port_data & (~UHCI_PORTSC_PORT_ENABLE_CHANGED | ~UHCI_PORTSC_CONNECT_STATUS_CHANGED));

                            port_data = read_portsc1();
                            write_portsc1(port_data | UHCI_PORTSC_PORT_ENABLED);
                            dbgln("port should be enabled now: {:#04x}\n", read_portsc1());

                            USB::Device::DeviceSpeed speed = (port_data & UHCI_PORTSC_LOW_SPEED_DEVICE) ? USB::Device::DeviceSpeed::LowSpeed : USB::Device::DeviceSpeed::FullSpeed;
                            auto device = USB::Device::try_create(USB::Device::PortNumber::Port1, speed);

                            if (device.is_error())
                                dmesgln("UHCI: Device creation failed on port 1 ({})", device.error());

                            m_devices.at(0) = device.value();
                            VERIFY(s_procfs_usb_bus_directory);
                            s_procfs_usb_bus_directory->plug(device.value());
                        } else {
                            VERIFY(s_procfs_usb_bus_directory);
                            VERIFY(m_devices.at(0));
                            dmesgln("UHCI: Device detach detected on Root Port 1");
                            s_procfs_usb_bus_directory->unplug(*m_devices.at(0));
                        }
                    }
                } else {
                    port_data = UHCIController::the().read_portsc2();
                    if (port_data & UHCI_PORTSC_CONNECT_STATUS_CHANGED) {
                        if (port_data & UHCI_PORTSC_CURRRENT_CONNECT_STATUS) {
                            dmesgln("UHCI: Device attach detected on Root Port 2");

                            port_data = read_portsc2();
                            write_portsc2(port_data | UHCI_PORTSC_PORT_RESET);
                            for (size_t i = 0; i < 50000; ++i)
                                IO::in8(0x80);

                            write_portsc2(port_data & ~UHCI_PORTSC_PORT_RESET);
                            for (size_t i = 0; i < 100000; ++i)
                                IO::in8(0x80);

                            write_portsc2(port_data & (~UHCI_PORTSC_PORT_ENABLE_CHANGED | ~UHCI_PORTSC_CONNECT_STATUS_CHANGED));

                            port_data = read_portsc2();
                            write_portsc1(port_data | UHCI_PORTSC_PORT_ENABLED);
                            dbgln("port should be enabled now: {:#04x}\n", read_portsc1());
                            USB::Device::DeviceSpeed speed = (port_data & UHCI_PORTSC_LOW_SPEED_DEVICE) ? USB::Device::DeviceSpeed::LowSpeed : USB::Device::DeviceSpeed::FullSpeed;
                            auto device = USB::Device::try_create(USB::Device::PortNumber::Port2, speed);

                            if (device.is_error())
                                dmesgln("UHCI: Device creation failed on port 2 ({})", device.error());

                            m_devices.at(1) = device.value();
                            VERIFY(s_procfs_usb_bus_directory);
                            s_procfs_usb_bus_directory->plug(device.value());
                        } else {
                            VERIFY(s_procfs_usb_bus_directory);
                            VERIFY(m_devices.at(1));
                            dmesgln("UHCI: Device detach detected on Root Port 2");
                            s_procfs_usb_bus_directory->unplug(*m_devices.at(1));
                        }
                    }
                }
            }
            (void)Thread::current()->sleep(Time::from_seconds(1));
        }
    });
}

bool UHCIController::handle_irq(const RegisterState&)
{
    u32 status = read_usbsts();

    if (!status)
        return false;

    if constexpr (UHCI_DEBUG) {
        dbgln("UHCI: Interrupt happened!");
        dbgln("Value of USBSTS: {:#04x}", read_usbsts());
    }

    write_usbsts(status);
    return true;
}

}