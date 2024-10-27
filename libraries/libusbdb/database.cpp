/**
 * @file database.cpp
 * @author Krisna Pranav
 * @brief Database
 * @version 6.0
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/ownptr.h>
#include <mods/refptr.h>
#include <mods/string_view.h>
#include "database.h"

namespace USBDB 
{

    /**
     * @param filename 
     * @return RefPtr<Database> 
     */
    RefPtr<Database> Database::open(const String& filename)
    {
        auto file_or_error = Core::MappedFile::map(filename);

        if (file_or_error.is_error())
            return nullptr;

        auto res = adopt_ref(*new Database(file_or_error.release_value()));

        if (res->init() != 0)
            return nullptr;
            
        return res;
    }

    /**
     * @param vendor_id 
     * @return const StringView 
     */
    const StringView Database::get_vendor(u16 vendor_id) const
    {
        const auto& vendor = m_vendors.get(vendor_id);

        if (!vendor.has_value())
            return "";

        return vendor.value()->name;
    }

    /**
     * @param vendor_id 
     * @param device_id 
     * @return const StringView 
     */
    const StringView Database::get_device(u16 vendor_id, u16 device_id) const
    {
        const auto& vendor = m_vendors.get(vendor_id);

        if (!vendor.has_value()) {
            return "";
        }

        const auto& device = vendor.value()->devices.get(device_id);

        if (!device.has_value())
            return "";

        return device.value()->name;
    }

    /**
     * @param vendor_id 
     * @param device_id 
     * @param interface_id 
     * @return const StringView 
     */
    const StringView Database::get_interface(u16 vendor_id, u16 device_id, u16 interface_id) const
    {
        const auto& vendor = m_vendors.get(vendor_id);

        if (!vendor.has_value())
            return "";

        const auto& device = vendor.value()->devices.get(device_id);

        if (!device.has_value())
            return "";

        const auto& interface = device.value()->interfaces.get(interface_id);

        if (!interface.has_value())
            return "";

        return interface.value()->name;
    }

    /**
     * @param class_id 
     * @return const StringView 
     */
    const StringView Database::get_class(u8 class_id) const
    {
        const auto& xclass = m_classes.get(class_id);

        if (!xclass.has_value())
            return "";

        return xclass.value()->name;
    }

    /**
     * @param class_id 
     * @param subclass_id 
     * @return const StringView 
     */
    const StringView Database::get_subclass(u8 class_id, u8 subclass_id) const
    {
        const auto& xclass = m_classes.get(class_id);

        if (!xclass.has_value())
            return "";

        const auto& subclass = xclass.value()->subclasses.get(subclass_id);

        if (!subclass.has_value())
            return "";

        return subclass.value()->name;
    }

    /**
     * @param class_id 
     * @param subclass_id 
     * @param protocol_id 
     * @return const StringView 
     */
    const StringView Database::get_protocol(u8 class_id, u8 subclass_id, u8 protocol_id) const
    {
        const auto& xclass = m_classes.get(class_id);

        if (!xclass.has_value())
            return "";

        const auto& subclass = xclass.value()->subclasses.get(subclass_id);

        if (!subclass.has_value())
            return "";

        const auto& protocol = subclass.value()->protocols.get(protocol_id);

        if (!protocol.has_value())
            return "";

        return protocol.value()->name;
    }

    int Database::init()
    {
        if (m_ready)
            return 0;

        m_view = StringView { m_file->bytes() };

        ParseMode mode = ParseMode::UnknownMode;

        OwnPtr<Vendor> current_vendor {};
        OwnPtr<Device> current_device {};
        OwnPtr<Class> current_class {};
        OwnPtr<Subclass> current_subclass {};

        auto commit_device = [&]() {
            if (current_device && current_vendor) {
                auto id = current_device->id;
                current_vendor->devices.set(id, current_device.release_nonnull());
            }
        };

        auto commit_vendor = [&]() {
            commit_device();
            if (current_vendor) {
                auto id = current_vendor->id;
                m_vendors.set(id, current_vendor.release_nonnull());
            }
        };

        auto commit_subclass = [&]() {
            if (current_subclass && current_class) {
                auto id = current_subclass->id;
                current_class->subclasses.set(id, current_subclass.release_nonnull());
            }
        };

        auto commit_class = [&]() {
            commit_subclass();
            if (current_class) {
                auto id = current_class->id;
                m_classes.set(id, current_class.release_nonnull());
            }
        };

        auto commit_all = [&]() {
            commit_vendor();
            commit_class();
        };

        auto lines = m_view.split_view('\n');

        for (auto& line : lines) {
            if (line.length() < 2 || line[0] == '#')
                continue;

            if (line[0] == 'C') {
                mode = ParseMode::ClassMode;
                commit_all();
            } else if ((line[0] >= '0' && line[0] <= '9') || (line[0] >= 'a' && line[0] <= 'f')) {
                mode = ParseMode::VendorMode;
                commit_all();
            } else if (line[0] != '\t') {
                mode = ParseMode::UnknownMode;
                continue;
            }

            switch (mode) {
            case ParseMode::VendorMode:
                if (line[0] != '\t') {
                    commit_vendor();
                    current_vendor = make<Vendor>();
                    current_vendor->id = Mods::StringUtils::convert_to_uint_from_hex<u16>(line.substring_view(0, 4)).value_or(0);
                    current_vendor->name = line.substring_view(6, line.length() - 6);
                } else if (line[0] == '\t' && line[1] != '\t') {
                    commit_device();
                    current_device = make<Device>();
                    current_device->id = Mods::StringUtils::convert_to_uint_from_hex<u16>((line.substring_view(1, 4))).value_or(0);
                    current_device->name = line.substring_view(7, line.length() - 7);
                } else if (line[0] == '\t' && line[1] == '\t') {
                    auto interface = make<Interface>();
                    interface->interface = Mods::StringUtils::convert_to_uint_from_hex<u16>((line.substring_view(2, 4))).value_or(0);
                    interface->name = line.substring_view(7, line.length() - 7);
                    current_device->interfaces.set(interface->interface, move(interface));
                }
                break;
            case ParseMode::ClassMode:
                if (line[0] != '\t') {
                    commit_class();
                    current_class = make<Class>();
                    current_class->id = Mods::StringUtils::convert_to_uint_from_hex<u16>((line.substring_view(2, 2))).value_or(0);
                    current_class->name = line.substring_view(6, line.length() - 6);
                } else if (line[0] == '\t' && line[1] != '\t') {
                    commit_subclass();
                    current_subclass = make<Subclass>();
                    current_subclass->id = Mods::StringUtils::convert_to_uint_from_hex<u16>((line.substring_view(1, 2))).value_or(0);
                    current_subclass->name = line.substring_view(5, line.length() - 5);
                } else if (line[0] == '\t' && line[1] == '\t') {
                    auto protocol = make<Protocol>();
                    protocol->id = Mods::StringUtils::convert_to_uint_from_hex<u16>((line.substring_view(2, 2))).value_or(0);
                    protocol->name = line.substring_view(6, line.length() - 6);
                    current_subclass->protocols.set(protocol->id, move(protocol));
                }
                break;
            default:
                break;
            }
        }

        commit_all();

        m_ready = true;

        return 0;
    }
} // namespace USBDB