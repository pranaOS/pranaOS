/**
 * @file kstdio.h
 * @author Krisna Pranav
 * @brief KSTDIO
 * @version 6.0
 * @date 2023-07-01
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/format.h>
#include <mods/ownptr.h>

namespace Kernel
{
    class KString
    {
        MOD_MAKE_NONCOPYABLE(KString);
        MOD_MAKE_NONMOVABLE(KString);

      public:

        /**
         * @brief try_create_uninitialized
         */
        [[nodiscard]] static ErrorOr<NonnullOwnPtr<KString>> try_create_uninitialized(size_t, char*&);

        /**
         * @brief must_create_uninitialized
         */
        [[nodiscard]] static NonnullOwnPtr<KString> must_create_uninitialized(size_t, char*&);

        /**
         * @brief try_create
         */
        [[nodiscard]] static ErrorOr<NonnullOwnPtr<KString>> try_create(StringView);

        /**
         * @brief must_create
         */
        [[nodiscard]] static NonnullOwnPtr<KString> must_create(StringView);

        /**
         * @brief vformatted
         * @param fmtstr
         */
        [[nodiscard]] static ErrorOr<NonnullOwnPtr<KString>> vformatted(StringView fmtstr, Mods::TypeErasedFormatParams&);

        /**
         * @brief formatted
         * @param fmtstr
         * @param parameters
         */
        template<typename... Parameters>
        [[nodiscard]] static ErrorOr<NonnullOwnPtr<KString>> formatted(CheckedFormatString<Parameters...>&& fmtstr, const Parameters&... parameters)
        {
            Mods::VariadicFormatParams variadic_format_parameters { parameters... };
            return vformatted(fmtstr.view(), variadic_format_parameters);
        }

        /**
         * @param value
         */
        [[nodiscard]] static ErrorOr<NonnullOwnPtr<KString>> number(Arithmetic auto value)
        {
            return formatted("{}", value);
        }

        void operator delete(void*);

        ErrorOr<NonnullOwnPtr<KString>> try_clone() const;

        /**
         * @brief is_empty
         */
        [[nodiscard]] bool is_empty() const
        {
            return m_length == 0;
        }

        [[nodiscard]] size_t length() const
        {
            return m_length;
        }

        [[nodiscard]] char const* characters() const
        {
            return m_characters;
        }

        [[nodiscard]] StringView view() const
        {
            return { characters(), length() };
        }

        [[nodiscard]] ReadonlyBytes bytes() const
        {
            return { characters(), length() };
        }

      private:
        explicit KString(size_t length)
            : m_length(length)
        {
        }

        size_t m_length { 0 };

        char m_characters[0];
    }; // class KString

} // namespace Kernel

namespace Mods
{
    template<>
    struct Formatter<Kernel::KString> : Formatter<StringView>
    {
        ErrorOr<void> format(FormatBuilder& builder, Kernel::KString const& value)
        {
            return Formatter<StringView>::format(builder, value.view());
        }
    };

    /**
     * @brief The Formatter class
     */
    template<>
    struct Formatter<OwnPtr<Kernel::KString>> : Formatter<StringView>
    {
        ErrorOr<void> format(FormatBuilder& builder, OwnPtr<Kernel::KString> const& value)
        {
            if (value)
                return Formatter<StringView>::format(builder, value->view());
            return Formatter<StringView>::format(builder, "[out of memory]"sv);
        }
    };

    /**
     * @brief The Formatter class
     */
    template<>
    struct Formatter<NonnullOwnPtr<Kernel::KString>> : Formatter<StringView>
    {
        ErrorOr<void> format(FormatBuilder& builder, NonnullOwnPtr<Kernel::KString> const& value)
        {
            return Formatter<StringView>::format(builder, value->view());
        }
    };

    /**
     * @brief The Traits class
     */
    template<>
    struct Traits<NonnullOwnPtr<Kernel::KString>> : public GenericTraits<NonnullOwnPtr<Kernel::KString>>
    {
        using PeekType = Kernel::KString*;
        using ConstPeekType = Kernel::KString const*;

        /**
         * @brief hash
         * @param p
         */
        static unsigned hash(NonnullOwnPtr<Kernel::KString> const& p)
        {
            return string_hash(p->characters(), p->length());
        }

        /**
         * @brief equals
         * @param a
         * @param b
         */
        static bool equals(NonnullOwnPtr<Kernel::KString> const& a, NonnullOwnPtr<Kernel::KString> const& b)
        {
            return a->view() == b->view();
        }

        /**
         * @brief equals
         * @param a
         * @param b
         */
        static bool equals(StringView a, NonnullOwnPtr<Kernel::KString> const& b)
        {
            return a == b->view();
        }
    };

    /**
     * @brief The Traits class
     */
    template<>
    struct Traits<OwnPtr<Kernel::KString>> : public GenericTraits<OwnPtr<Kernel::KString>>
    {
        using PeekType = Kernel::KString*;
        using ConstPeekType = Kernel::KString const*;

        /**
         * @brief hash
         * @param p
         */
        static unsigned hash(OwnPtr<Kernel::KString> const& p)
        {
            if (!p)
                return ptr_hash(nullptr);
            return string_hash(p->characters(), p->length());
        }

        /**
         * @brief equals
         * @param a
         * @param b
         */
        static bool equals(OwnPtr<Kernel::KString> const& a, OwnPtr<Kernel::KString> const& b)
        {
            if (!a || !b)
                return a.ptr() == b.ptr();
            if (a == b)
                return true;

            return a->view() == b->view();
        }

        /**
         * @brief equals
         * @param a
         * @param b
         */
        static bool equals(StringView a, OwnPtr<Kernel::KString> const& b)
        {
            if (!b)
                return a.is_null();
            return a == b->view();
        }
    };

    namespace Detail
    {
        template<>
        inline constexpr bool IsHashCompatible<StringView, NonnullOwnPtr<Kernel::KString>> = true;

        template<>
        inline constexpr bool IsHashCompatible<StringView, OwnPtr<Kernel::KString>> = true;
    } // namespace Detail

} // namespace Mods
