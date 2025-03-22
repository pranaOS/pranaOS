/**
 * @file fixedstringbuffer.h
 * @author Krisna Pranav
 * @brief Fixed String Buffer
 * @version 6.0
 * @date 2025-03-22
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/array.h>
#include <mods/stringbuilder.h>
#include <mods/stringview.h>
#include <mods/typedtransfer.h>
#include <mods/userspace.h>

#ifdef KERNEL
#    include <kernel/arch/safemem.h>
#    include <kernel/arch/smapdisabler.h>
#    include <kernel/memory/memorysections.h>
#endif

namespace Mods 
{

    /**
     * @tparam Size 
     */
    template<size_t Size>
    class FixedStringBuffer 
    {
    public:
        [[nodiscard]] static ErrorOr<FixedStringBuffer<Size>> vformatted(StringView fmtstr, Mods::TypeErasedFormatParams& params)
        requires(Size < StringBuilder::inline_capacity)
        {
            StringBuilder builder { StringBuilder::UseInlineCapacityOnly::Yes };
            TRY(Mods::vformat(builder, fmtstr, params));
            FixedStringBuffer<Size> buffer {};
            buffer.store_characters(builder.string_view());
            return buffer;
        }

        /**
         * @tparam Parameters 
         */
        template<typename... Parameters>
        [[nodiscard]] static ErrorOr<FixedStringBuffer<Size>> formatted(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
        requires(Size < StringBuilder::inline_capacity)
        {
            Mods::VariadicFormatParams<Mods::AllowDebugOnlyFormatters::No, Parameters...> variadic_format_parameters { parameters... };
            return vformatted(fmtstr.view(), variadic_format_parameters);
        }

        /**
         * @param characters 
         */
        void store_characters(StringView characters)
        {
            size_t stored_length = 0;
            for (; stored_length < min(Size, characters.length()); stored_length++) {
                if (characters[stored_length] == '\0')
                    break;
                m_storage[stored_length] = characters[stored_length];
            }
            m_stored_length = stored_length;

            for (size_t index = m_stored_length; index < Size; index++)
                m_storage[index] = '\0';
        }

    #ifdef KERNEL
        /**
         * @param user_str 
         * @param user_str_size 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> copy_characters_from_user(Userspace<char const*> user_str, size_t user_str_size)
        {
            if (user_str_size > Size)
                return EFAULT;
            bool is_user = Kernel::Memory::is_user_range(user_str.vaddr(), user_str_size);
            if (!is_user)
                return EFAULT;
            Kernel::SmapDisabler disabler;
            void* fault_at;
            ssize_t length = Kernel::safe_strnlen(user_str.unsafe_userspace_ptr(), user_str_size, fault_at);
            if (length < 0) {
                dbgln("FixedStringBuffer::copy_characters_into_storage({:p}, {}) failed at {} (strnlen)", static_cast<void const*>(user_str.unsafe_userspace_ptr()), user_str_size, VirtualAddress { fault_at });
                return EFAULT;
            }
            if (!Kernel::safe_memcpy(m_storage.data(), user_str.unsafe_userspace_ptr(), (size_t)length, fault_at)) {
                dbgln("FixedStringBuffer::copy_characters_into_storage({:p}, {}) failed at {} (memcpy)", static_cast<void const*>(user_str.unsafe_userspace_ptr()), user_str_size, VirtualAddress { fault_at });
                return EFAULT;
            }
            m_stored_length = (size_t)length;
            for (size_t index = m_stored_length; index < Size; index++)
                m_storage[index] = '\0';
            return {};
        }
    #endif

        /**
         * @return Span<u8> 
         */
        Span<u8> storage()
        {
            return m_storage.span();
        }

        /**
         * @return StringView 
         */
        StringView representable_view() const 
        { 
            return StringView(m_storage.data(), m_stored_length); 
        }

        /**
         * @return Span<u8 const> 
         */
        Span<u8 const> span_view_ensuring_ending_null_char()
        {
            VERIFY(m_stored_length + 1 <= Size);
            m_storage[m_stored_length] = '\0';
            return Span<u8 const>(m_storage.data(), m_stored_length + 1);
        }

        /**
         * @return size_t 
         */
        size_t stored_length() const 
        { 
            return m_stored_length; 
        }

        FixedStringBuffer()
        {
            m_storage.fill(0);
        }

    private:
        Array<u8, Size> m_storage;
        size_t m_stored_length { 0 };
    }; // class FixedStringBuffer

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::FixedStringBuffer;
#endif
