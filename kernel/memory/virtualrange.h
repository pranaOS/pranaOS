/**
 * @file virtualrange.h
 * @author Krisna Pranav
 * @brief Virtual Range
 * @version 6.0
 * @date 2024-10-19
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <kernel/virtual_address.h>

namespace Kernel::Memory 
{
    class VirtualRange 
    {
        friend class VirtualRangeAllocator;

    public:
        /**
         * @brief Construct a new Virtual Range object
         * 
         */
        VirtualRange() = delete;

        /**
         * @brief Construct a new Virtual Range object
         * 
         * @param base 
         * @param size 
         */
        VirtualRange(VirtualAddress base, size_t size)
            : m_base(base)
            , m_size(size)
        {
        }

        /**
         * @return VirtualAddress 
         */
        VirtualAddress base() const 
        { 
            return m_base; 
        }

        size_t size() const 
        { 
            return m_size; 
        }

        bool is_valid() const 
        { 
            return !m_base.is_null(); 
        }

        /**
         * @param vaddr 
         * @return true 
         * @return false 
         */
        bool contains(VirtualAddress vaddr) const 
        { 
            return vaddr >= base() && vaddr < end(); 
        }

        /**
         * @return VirtualAddress 
         */
        VirtualAddress end() const 
        { 
            return m_base.offset(m_size); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(VirtualRange const& other) const
        {
            return m_base == other.m_base && m_size == other.m_size;
        }       

        /**
         * @param base 
         * @param size 
         * @return true 
         * @return false 
         */
        bool contains(VirtualAddress base, size_t size) const
        {
            if (base.offset(size) < base)
                return false;

            return base >= m_base && base.offset(size) <= end();
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool contains(VirtualRange const& other) const
        {
            return contains(other.base(), other.size());
        }

        /**
         * @return Vector<VirtualRange, 2> 
         */
        Vector<VirtualRange, 2> carve(VirtualRange const&) const;

        /**
         * @return VirtualRange 
         */
        VirtualRange intersect(VirtualRange const&) const;

        /**
         * @param address 
         * @param size 
         * @return ErrorOr<VirtualRange> 
         */
        static ErrorOr<VirtualRange> expand_to_page_boundaries(FlatPtr address, size_t size);

    private:
        VirtualAddress m_base;
        size_t m_size { 0 };
    }; // class VirtualRange

} // namespace Kernel::Memory

template<>
struct Mods::Formatter<Kernel::Memory::VirtualRange> : Formatter<FormatString> 
{
    /**
     * @param builder 
     * @param value 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> format(FormatBuilder& builder, Kernel::Memory::VirtualRange value)
    {
        return Formatter<FormatString>::format(builder, "{} - {} (size {:p})", value.base().as_ptr(), value.base().offset(value.size() - 1).as_ptr(), value.size());
    }
}; // struct Mods::Formatter<Kernel::Memory::VirtualRange> : Formatter<FormatString> 