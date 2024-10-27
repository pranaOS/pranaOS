/**
 * @file database.h
 * @author Krisna Pranav
 * @brief Database
 * @version 6.0
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/hashmap.h>
#include <mods/nonnullownptr.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/string.h>
#include <mods/string_view.h>
#include <libcore/mappedfile.h>

namespace USBDB 
{

    struct Interface 
    {
        u16 interface;
        StringView name;
    }; // struct Interface 

    struct Device 
    {
        u16 id;
        StringView name;
        HashMap<int, NonnullOwnPtr<Interface>> interfaces;
    }; // struct Device 

    struct Vendor 
    {
        u16 id;
        StringView name;
        HashMap<int, NonnullOwnPtr<Device>> devices;
    }; // struct Vendor 

    struct Protocol 
    {
        u8 id { 0 };
        StringView name {};
    }; // struct Protocol 

    struct Subclass 
    {
        u8 id { 0 };
        StringView name {};
        HashMap<int, NonnullOwnPtr<Protocol>> protocols;
    }; // struct Subclass 

    struct Class 
    {
        u8 id { 0 };
        StringView name {};
        HashMap<int, NonnullOwnPtr<Subclass>> subclasses;
    }; // struct Class 

    class Database : public RefCounted<Database> 
    {
    public:
        /**
         * @param filename 
         * @return RefPtr<Database> 
         */
        static RefPtr<Database> open(const String& filename);

        static RefPtr<Database> open() 
        { 
            return open("/res/usb.ids"); 
        };

        /**
         * @brief Get the vendor object
         * 
         * @param vendor_id 
         * @return const StringView 
         */
        const StringView get_vendor(u16 vendor_id) const;

        /**
         * @brief Get the device object
         * 
         * @param vendor_id 
         * @param device_id 
         * @return const StringView 
         */
        const StringView get_device(u16 vendor_id, u16 device_id) const;

        /**
         * @brief Get the interface object
         * 
         * @param vendor_id 
         * @param device_id 
         * @param interface_id 
         * @return const StringView 
         */
        const StringView get_interface(u16 vendor_id, u16 device_id, u16 interface_id) const;

        /**
         * @brief Get the class object
         * 
         * @param class_id 
         * @return const StringView 
         */
        const StringView get_class(u8 class_id) const;

        /**
         * @brief Get the subclass object
         * 
         * @param class_id 
         * @param subclass_id 
         * @return const StringView 
         */
        const StringView get_subclass(u8 class_id, u8 subclass_id) const;
        
        /**
         * @brief Get the protocol object
         * 
         * @param class_id 
         * @param subclass_id 
         * @param protocol_id 
         * @return const StringView 
         */
        const StringView get_protocol(u8 class_id, u8 subclass_id, u8 protocol_id) const;

    private:
        /**
         * @brief Construct a new Database object
         * 
         * @param file 
         */
        explicit Database(NonnullRefPtr<Core::MappedFile> file)
            : m_file(move(file))
        {
        }

        int init();

        enum ParseMode 
        {
            UnknownMode,
            VendorMode,
            ClassMode,
        }; // enum ParseMode 

        NonnullRefPtr<Core::MappedFile> m_file;
        StringView m_view {};
        HashMap<int, NonnullOwnPtr<Vendor>> m_vendors;
        HashMap<int, NonnullOwnPtr<Class>> m_classes;
        bool m_ready { false };
    }; // class Database : public RefCounted<Database> 
} // namespace USBDB