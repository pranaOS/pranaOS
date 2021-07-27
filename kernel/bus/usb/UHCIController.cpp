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

}