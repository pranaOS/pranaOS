/**
 * @file format.h
 * @author Krisna Pranav
 * @brief format
 * @version 6.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/checkedformatstring.h>
#include <mods/allof.h>
#include <mods/anyof.h>
#include <mods/error.h>
#include <mods/forward.h>
#include <mods/optional.h>
#include <mods/stringview.h>

#ifndef KERNEL
#    include <stdio.h>
#    include <string.h>
#endif

namespace Mods 
{

    class TypeErasedFormatParams;
    class FormatParser;
    class FormatBuilder;

    /**
     * @tparam T 
     * @tparam typename 
     */
    template<typename T, typename = void>
    struct Formatter 
    {
        using __no_formatter_defined = void;
    }; // struct Formatter

    enum AllowDebugOnlyFormatters 
    {
        No,
        Yes
    }; // enum AllowDebugOnlyFormatters 

    /**
     * @tparam T 
     * @tparam typename 
     */
    template<typename T, typename = void>
    inline constexpr bool HasFormatter = true;

    /**
     * @tparam T 
     */
    template<typename T>
    inline constexpr bool HasFormatter<T, typename Formatter<T>::__no_formatter_defined> = false;

    /**
     * @tparam Formatter 
     * @return true 
     * @return false 
     */
    template<typename Formatter>
    inline constexpr bool is_debug_only_formatter()
    {
        constexpr bool has_is_debug_only = requires(Formatter const& formatter) { formatter.is_debug_only(); };
        if constexpr (has_is_debug_only)
            return Formatter::is_debug_only();
        return false;
    }

    /**
     * @tparam T 
     */
    template<typename T>
    concept Formattable = HasFormatter<T>;

    constexpr size_t max_format_arguments = 256;

    struct TypeErasedParameter 
    {
        enum class Type 
        {
            UInt8,
            UInt16,
            UInt32,
            UInt64,
            Int8,
            Int16,
            Int32,
            Int64,
            Custom
        }; // enum class Type

        /**
         * @brief Get the type from size object
         * 
         * @tparam size 
         * @tparam is_unsigned 
         * @return consteval 
         */
        template<size_t size, bool is_unsigned>
        static consteval Type get_type_from_size()
        {
            if constexpr (is_unsigned) {
                if constexpr (size == 1)
                    return Type::UInt8;
                if constexpr (size == 2)
                    return Type::UInt16;
                if constexpr (size == 4)
                    return Type::UInt32;
                if constexpr (size == 8)
                    return Type::UInt64;
            } else {
                if constexpr (size == 1)
                    return Type::Int8;
                if constexpr (size == 2)
                    return Type::Int16;
                if constexpr (size == 4)
                    return Type::Int32;
                if constexpr (size == 8)
                    return Type::Int64;
            }

            VERIFY_NOT_REACHED();
        }

        /**
         * @brief Get the type object
         * 
         * @tparam T 
         * @return consteval 
         */
        template<typename T>
        static consteval Type get_type()
        {
            if constexpr (IsIntegral<T>)
                return get_type_from_size<sizeof(T), IsUnsigned<T>>();
            else
                return Type::Custom;
        }

        /**
         * @tparam Visitor 
         * @param visitor 
         * @return constexpr auto 
         */
        template<typename Visitor>
        constexpr auto visit(Visitor&& visitor) const
        {
            switch (type) {
            case TypeErasedParameter::Type::UInt8:
                return visitor(*static_cast<u8 const*>(value));
            case TypeErasedParameter::Type::UInt16:
                return visitor(*static_cast<u16 const*>(value));
            case TypeErasedParameter::Type::UInt32:
                return visitor(*static_cast<u32 const*>(value));
            case TypeErasedParameter::Type::UInt64:
                return visitor(*static_cast<u64 const*>(value));
            case TypeErasedParameter::Type::Int8:
                return visitor(*static_cast<i8 const*>(value));
            case TypeErasedParameter::Type::Int16:
                return visitor(*static_cast<i16 const*>(value));
            case TypeErasedParameter::Type::Int32:
                return visitor(*static_cast<i32 const*>(value));
            case TypeErasedParameter::Type::Int64:
                return visitor(*static_cast<i64 const*>(value));
            default:
                TODO();
            }
        }

        /**
         * @return constexpr size_t 
         */
        constexpr size_t to_size() const
        {
            return visit([]<typename T>(T value) {
                if constexpr (sizeof(T) > sizeof(size_t))
                    VERIFY(value < NumericLimits<size_t>::max());
                if constexpr (IsSigned<T>)
                    VERIFY(value >= 0);
                return static_cast<size_t>(value);
            });
        }

        void const* value;
        Type type;
        ErrorOr<void> (*formatter)(TypeErasedFormatParams&, FormatBuilder&, FormatParser&, void const* value);
    }; // st ruct TypeErasedParameter

    class FormatBuilder 
    {
    public:
        enum class Align 
        {
            Default,
            Left,
            Center,
            Right,
        }; // enum class Align

        enum class SignMode 
        {
            OnlyIfNeeded,
            Always,
            Reserved,
            Default = OnlyIfNeeded,
        }; // enum class SignMode

        enum class RealNumberDisplayMode 
        {
            FixedPoint,
            General,
            Default = General,
        }; // enum class RealNumberDisplayMode

        /**
         * @brief Construct a new Format Builder object
         * 
         * @param builder 
         */
        explicit FormatBuilder(StringBuilder& builder)
            : m_builder(builder)
        {
        }

        /**
         * @param fill 
         * @param amount 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> put_padding(char fill, size_t amount);

        /**
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> put_literal(StringView value);

        /**
         * @param value 
         * @param align 
         * @param min_width 
         * @param max_width 
         * @param fill 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> put_string(
            StringView value,
            Align align = Align::Left,
            size_t min_width = 0,
            size_t max_width = NumericLimits<size_t>::max(),
            char fill = ' ');

        /**
         * @param value 
         * @param base 
         * @param prefix 
         * @param upper_case 
         * @param zero_pad 
         * @param use_separator 
         * @param align 
         * @param min_width 
         * @param fill 
         * @param sign_mode 
         * @param is_negative 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> put_u64(
            u64 value,
            u8 base = 10,
            bool prefix = false,
            bool upper_case = false,
            bool zero_pad = false,
            bool use_separator = false,
            Align align = Align::Right,
            size_t min_width = 0,
            char fill = ' ',
            SignMode sign_mode = SignMode::OnlyIfNeeded,
            bool is_negative = false);

        /**
         * @param value 
         * @param base 
         * @param prefix 
         * @param upper_case 
         * @param zero_pad 
         * @param use_separator 
         * @param align 
         * @param min_width 
         * @param fill 
         * @param sign_mode 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> put_i64(
            i64 value,
            u8 base = 10,
            bool prefix = false,
            bool upper_case = false,
            bool zero_pad = false,
            bool use_separator = false,
            Align align = Align::Right,
            size_t min_width = 0,
            char fill = ' ',
            SignMode sign_mode = SignMode::OnlyIfNeeded);

        /**
         * @param is_negative 
         * @param integer_value 
         * @param fraction_value 
         * @param fraction_one 
         * @param precision 
         * @param base 
         * @param upper_case 
         * @param zero_pad 
         * @param use_separator 
         * @param align 
         * @param min_width 
         * @param fraction_max_width 
         * @param fill 
         * @param sign_mode 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> put_fixed_point(
            bool is_negative,
            i64 integer_value,
            u64 fraction_value,
            u64 fraction_one,
            size_t precision,
            u8 base = 10,
            bool upper_case = false,
            bool zero_pad = false,
            bool use_separator = false,
            Align align = Align::Right,
            size_t min_width = 0,
            size_t fraction_max_width = 6,
            char fill = ' ',
            SignMode sign_mode = SignMode::OnlyIfNeeded);

    #ifndef KERNEL
        /**
         * @param value 
         * @param base 
         * @param upper_case 
         * @param use_separator 
         * @param align 
         * @param min_width 
         * @param precision 
         * @param fill 
         * @param sign_mode 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> put_f80(
            long double value,
            u8 base = 10,
            bool upper_case = false,
            bool use_separator = false,
            Align align = Align::Right,
            size_t min_width = 0,
            size_t precision = 6,
            char fill = ' ',
            SignMode sign_mode = SignMode::OnlyIfNeeded,
            RealNumberDisplayMode = RealNumberDisplayMode::Default);

        /**
         * @tparam T 
         * @param value 
         * @param base 
         * @param upper_case 
         * @param zero_pad 
         * @param use_separator 
         * @param align 
         * @param min_width 
         * @param precision 
         * @param fill 
         * @param sign_mode 
         * @return ErrorOr<void> 
         */
        template<OneOf<f32, f64> T>
        ErrorOr<void> put_f32_or_f64(
            T value,
            u8 base = 10,
            bool upper_case = false,
            bool zero_pad = false,
            bool use_separator = false,
            Align align = Align::Right,
            size_t min_width = 0,
            Optional<size_t> precision = {},
            char fill = ' ',
            SignMode sign_mode = SignMode::OnlyIfNeeded,
            RealNumberDisplayMode = RealNumberDisplayMode::Default);
    #endif

        /**
         * @param width 
         * @param fill 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> put_hexdump(
            ReadonlyBytes,
            size_t width,
            char fill = ' ');

        /**
         * @return StringBuilder const& 
         */
        StringBuilder const& builder() const
        {
            return m_builder;
        }

        /**
         * @return StringBuilder& 
         */
        StringBuilder& builder() 
        { 
            return m_builder; 
        }

    private:
        StringBuilder& m_builder;

    #ifndef KERNEL
        /**
         * @param value 
         * @param base 
         * @param upper_case 
         * @param zero_pad 
         * @param use_separator 
         * @param align 
         * @param min_width 
         * @param precision 
         * @param fill 
         * @param sign_mode 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> put_f64_with_precision(
            double value,
            u8 base,
            bool upper_case,
            bool zero_pad,
            bool use_separator,
            Align align,
            size_t min_width,
            size_t precision,
            char fill,
            SignMode sign_mode,
            RealNumberDisplayMode);
    #endif
    }; // class FormatBuilder

    class TypeErasedFormatParams 
    {
    public:
        /**
         * @brief Construct a new TypeErasedFormatParams object
         * 
         * @param size 
         */
        TypeErasedFormatParams(u32 size)
            : m_size(size)
        {
        }

        /**
         * @return ReadonlySpan<TypeErasedParameter> 
         */
        ReadonlySpan<TypeErasedParameter> parameters() const 
        { 
            return { m_parameters, m_size }; 
        }

        /**
         * @return size_t 
         */
        size_t take_next_index() 
        { 
            return m_next_index++; 
        }

    private:
        u32 m_size { 0 };
        u32 m_next_index { 0 };
        TypeErasedParameter m_parameters[0];
    }; // class TypeErasedFormatParams

    /**
     * @tparam T 
     * @param params 
     * @param builder 
     * @param parser 
     * @param value 
     * @return ErrorOr<void> 
     */
    template<typename T>
    ErrorOr<void> __format_value(TypeErasedFormatParams& params, FormatBuilder& builder, FormatParser& parser, void const* value)
    {
        Formatter<T> formatter;

        formatter.parse(params, parser);
        return formatter.format(builder, *static_cast<T const*>(value));
    }

    /**
     * @tparam allow_debug_formatters 
     * @tparam Parameters 
     */
    template<AllowDebugOnlyFormatters allow_debug_formatters, typename... Parameters>
    class VariadicFormatParams : public TypeErasedFormatParams 
    {
    public:
        static_assert(sizeof...(Parameters) <= max_format_arguments);

        /**
         * @brief Construct a new Variadic Format Params object
         * 
         * @param parameters 
         */
        explicit VariadicFormatParams(Parameters const&... parameters)
            : TypeErasedFormatParams(sizeof...(Parameters))
            , m_parameter_storage { TypeErasedParameter { &parameters, TypeErasedParameter::get_type<Parameters>(), __format_value<Parameters> }... }
        {
            constexpr bool any_debug_formatters = (is_debug_only_formatter<Formatter<Parameters>>() || ...);
            static_assert(!any_debug_formatters || allow_debug_formatters == AllowDebugOnlyFormatters::Yes,
                "You are attempting to use a debug-only formatter outside of a debug log! Maybe one of your format values is an ErrorOr<T>?");
        }

    private:
        TypeErasedParameter m_parameter_storage[sizeof...(Parameters)];
    }; // class VariadicFormatParams : public TypeErasedFormatParams

    struct StandardFormatter 
    {
        enum class Mode 
        {
            Default,
            Binary,
            BinaryUppercase,
            Decimal,
            Octal,
            Hexadecimal,
            HexadecimalUppercase,
            Character,
            String,
            Pointer,
            FixedPoint,
            Hexfloat,
            HexfloatUppercase,
            HexDump,
        }; // enum class Mode 

        FormatBuilder::Align m_align = FormatBuilder::Align::Default;
        FormatBuilder::SignMode m_sign_mode = FormatBuilder::SignMode::OnlyIfNeeded;
        Mode m_mode = Mode::Default;
        bool m_alternative_form = false;
        bool m_use_separator = false;
        char m_fill = ' ';
        bool m_zero_pad = false;
        Optional<size_t> m_width;
        Optional<size_t> m_precision;

        void parse(TypeErasedFormatParams&, FormatParser&);
    }; // struct StandardFormatter 

    /**
     * @tparam T 
     */
    template<Integral T>
    struct Formatter<T> : StandardFormatter 
    {
        /**
         * @brief Construct a new Formatter object
         * 
         */
        Formatter() = default;
        
        /**
         * @brief Construct a new Formatter object
         * 
         * @param formatter 
         */
        explicit Formatter(StandardFormatter formatter)
            : StandardFormatter(move(formatter))
        {
        }

        ErrorOr<void> format(FormatBuilder&, T);
    }; // struct Formatter<T> : StandardFormatter 

    /**
     * @tparam  
     */
    template<>
    struct Formatter<StringView> : StandardFormatter 
    {
        /**
         * @brief Construct a new Formatter object
         * 
         */
        Formatter() = default;

        /**
         * @brief Construct a new Formatter object
         * 
         * @param formatter 
         */
        explicit Formatter(StandardFormatter formatter)
            : StandardFormatter(move(formatter))
        {
        }

        ErrorOr<void> format(FormatBuilder&, StringView);
    }; // struct Formatter<StringView> : StandardFormatter 

    /**
     * @brief Construct a new requires object
     * 
     * @tparam T 
     */
    template<typename T>
    requires(HasFormatter<T>)
    struct Formatter<ReadonlySpan<T>> : StandardFormatter 
    {
        /**
         * @brief Construct a new Formatter object
         * 
         */
        Formatter() = default;

        /**
         * @brief Construct a new Formatter object
         * 
         * @param formatter 
         */
        explicit Formatter(StandardFormatter formatter)
            : StandardFormatter(move(formatter))
        {
        }

        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, ReadonlySpan<T> value)
        {
            if (m_mode == Mode::Pointer) {
                Formatter<FlatPtr> formatter { *this };
                TRY(formatter.format(builder, reinterpret_cast<FlatPtr>(value.data())));
                return {};
            }

            if (m_sign_mode != FormatBuilder::SignMode::Default)
                VERIFY_NOT_REACHED();
            if (m_alternative_form)
                VERIFY_NOT_REACHED();
            if (m_zero_pad)
                VERIFY_NOT_REACHED();
            if (m_mode != Mode::Default)
                VERIFY_NOT_REACHED();
            if (m_width.has_value() && m_precision.has_value())
                VERIFY_NOT_REACHED();

            m_width = m_width.value_or(0);
            m_precision = m_precision.value_or(NumericLimits<size_t>::max());

            Formatter<T> content_fmt;
            TRY(builder.put_literal("[ "sv));
            bool first = true;
            for (auto& content : value) {
                if (!first) {
                    TRY(builder.put_literal(", "sv));
                    content_fmt = Formatter<T> {};
                }
                first = false;
                TRY(content_fmt.format(builder, content));
            }
            TRY(builder.put_literal(" ]"sv));
            return {};
        }
    };

    /**
     * @brief Construct a new requires object
     * 
     * @tparam T 
     */
    template<typename T>
    requires(HasFormatter<T>)
    struct Formatter<Span<T>> : Formatter<ReadonlySpan<T>> 
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, Span<T> value)
        {
            return Formatter<ReadonlySpan<T>>::format(builder, value);
        }
    };

    /**
     * @brief Construct a new requires object
     * 
     * @tparam T 
     * @tparam inline_capacity 
     */
    template<typename T, size_t inline_capacity>
    requires(HasFormatter<T>)
    struct Formatter<Vector<T, inline_capacity>> : Formatter<ReadonlySpan<T>> 
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, Vector<T, inline_capacity> const& value)
        {
            return Formatter<ReadonlySpan<T>>::format(builder, value.span());
        }
    };

    /**
     * @tparam  
     */
    template<>
    struct Formatter<ReadonlyBytes> : Formatter<StringView> 
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, ReadonlyBytes value)
        {
            if (m_mode == Mode::Pointer) {
                Formatter<FlatPtr> formatter { *this };
                return formatter.format(builder, reinterpret_cast<FlatPtr>(value.data()));
            }
            if (m_mode == Mode::Default || m_mode == Mode::HexDump) {
                m_mode = Mode::HexDump;
                return Formatter<StringView>::format(builder, value);
            }
            return Formatter<StringView>::format(builder, value);
        }
    };

    /**
     * @tparam  
     */
    template<>
    struct Formatter<Bytes> : Formatter<ReadonlyBytes> {
    };

    /**
     * @tparam  
     */
    template<>
    struct Formatter<char const*> : Formatter<StringView> 
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, char const* value)
        {
            if (m_mode == Mode::Pointer) {
                Formatter<FlatPtr> formatter { *this };
                return formatter.format(builder, reinterpret_cast<FlatPtr>(value));
            }

            return Formatter<StringView>::format(builder, value != nullptr ? StringView { value, __builtin_strlen(value) } : "(null)"sv);
        }
    };

    /**
     * @tparam  
     */
    template<>
    struct Formatter<char*> : Formatter<char const*> {
    };

    /**
     * @tparam Size 
     */
    template<size_t Size>
    struct Formatter<char[Size]> : Formatter<char const*> {
    };

    /**
     * @tparam Size 
     */
    template<size_t Size>
    struct Formatter<unsigned char[Size]> : Formatter<StringView> 
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, unsigned char const* value)
        {
            if (m_mode == Mode::Pointer) {
                Formatter<FlatPtr> formatter { *this };
                return formatter.format(builder, reinterpret_cast<FlatPtr>(value));
            }
            return Formatter<StringView>::format(builder, { value, Size });
        }
    };

    template<>
    struct Formatter<ByteString> : Formatter<StringView> {
    };

    template<>
    struct Formatter<DeprecatedFlyString> : Formatter<StringView> {
    };

    /**
     * @tparam T 
     */
    template<typename T>
    struct Formatter<T*> : StandardFormatter 
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, T* value)
        {
            if (m_mode == Mode::Default)
                m_mode = Mode::Pointer;

            Formatter<FlatPtr> formatter { *this };
            return formatter.format(builder, reinterpret_cast<FlatPtr>(value));
        }
    };

    template<>
    struct Formatter<char> : StandardFormatter 
    {
        ErrorOr<void> format(FormatBuilder&, char);
    }; // struct Formatter<char> : StandardFormatter 

    template<>
    struct Formatter<wchar_t> : StandardFormatter 
    {
        /**
         * @param builder 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, wchar_t);
    }; // struct Formatter<wchar_t> : StandardFormatter 
    
    template<>
    struct Formatter<bool> : StandardFormatter 
    {
        ErrorOr<void> format(FormatBuilder&, bool);
    }; // struct Formatter<bool> : StandardFormatter 

    #ifndef KERNEL
    template<>
    struct Formatter<float> : StandardFormatter 
    {
        /**
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder&, float value);
    }; // struct Formatter<float> : StandardFormatter 

    template<>
    struct Formatter<double> : StandardFormatter 
    {
        /**
         * @brief Construct a new Formatter object
         * 
         */
        Formatter() = default;

        /**
         * @brief Construct a new Formatter object
         * 
         * @param formatter 
         */
        explicit Formatter(StandardFormatter formatter)
            : StandardFormatter(formatter)
        {
        }

        ErrorOr<void> format(FormatBuilder&, double);
    }; // struct Formatter<double> : StandardFormatter 

    template<>
    struct Formatter<long double> : StandardFormatter 
    {
        /**
         * @brief Construct a new Formatter object
         * 
         */
        Formatter() = default;

        /**
         * @brief Construct a new Formatter object
         * 
         * @param formatter 
         */
        explicit Formatter(StandardFormatter formatter)
            : StandardFormatter(formatter)
        {
        }

        /**
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder&, long double value);
    }; // struct Formatter<long double> : StandardFormatter 
    #endif

    /**
     * @tparam  
     */
    template<>
    struct Formatter<nullptr_t> : Formatter<FlatPtr> 
    {
        /**
         * @param builder 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, nullptr_t)
        {
            if (m_mode == Mode::Default)
                m_mode = Mode::Pointer;

            return Formatter<FlatPtr>::format(builder, 0);
        }
    }; // struct Formatter<nullptr_t> : Formatter<FlatPtr> 

    /**
     * @param fmtstr 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> vformat(StringBuilder&, StringView fmtstr, TypeErasedFormatParams&);

    #if !defined(KERNEL)
    /**
     * @param fmtstr 
     * @param newline 
     */
    void vout(FILE*, StringView fmtstr, TypeErasedFormatParams&, bool newline = false);

    /**
     * @tparam Parameters 
     * @param file 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void out(FILE* file, CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
    {
        VariadicFormatParams<AllowDebugOnlyFormatters::Yes, Parameters...> variadic_format_params { parameters... };
        vout(file, fmtstr.view(), variadic_format_params);
    }

    /**
     * @tparam Parameters 
     * @param file 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void outln(FILE* file, CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
    {
        VariadicFormatParams<AllowDebugOnlyFormatters::Yes, Parameters...> variadic_format_params { parameters... };
        vout(file, fmtstr.view(), variadic_format_params, true);
    }

    /**
     * @param file 
     */
    inline void outln(FILE* file) 
    { 
        fputc('\n', file); 
    }

    #    ifndef MOD_OS_ANDROID
    /**
     * @tparam Parameters 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void out(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
    {
        out(stdout, move(fmtstr), parameters...);
    }

    /**
     * @tparam Parameters 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void outln(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters) { outln(stdout, move(fmtstr), parameters...); }

    inline void outln() 
    { 
        outln(stdout); 
    }

    /**
     * @tparam Parameters 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void warn(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
    {
        out(stderr, move(fmtstr), parameters...);
    }

    /**
     * @tparam Parameters 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void warnln(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters) 
    { 
        outln(stderr, move(fmtstr), parameters...); 
    }

    inline void warnln() { outln(stderr); }
    #    else  
    enum class LogLevel 
    {
        Debug,
        Info,
        Warning,
    }; // enum class LogLevel 

    /**
     * @param fmtstr 
     * @param newline 
     */
    void vout(LogLevel, StringView fmtstr, TypeErasedFormatParams&, bool newline = false);

    /**
     * @tparam Parameters 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void out(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
    {
        VariadicFormatParams<AllowDebugOnlyFormatters::Yes, Parameters...> variadic_format_params { parameters... };
        vout(LogLevel::Info, fmtstr.view(), variadic_format_params);
    }

    /**
     * @tparam Parameters 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void outln(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
    {
        VariadicFormatParams<AllowDebugOnlyFormatters::Yes, Parameters...> variadic_format_params { parameters... };
        vout(LogLevel::Info, fmtstr.view(), variadic_format_params, true);
    }

    inline void outln() 
    { 
        outln(""); 
    }

    /**
     * @tparam Parameters 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void warn(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
    {
        VariadicFormatParams<AllowDebugOnlyFormatters::Yes, Parameters...> variadic_format_params { parameters... };
        vout(LogLevel::Warning, fmtstr.view(), variadic_format_params);
    }

    /**
     * @tparam Parameters 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void warnln(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
    {
        VariadicFormatParams<AllowDebugOnlyFormatters::Yes, Parameters...> variadic_format_params { parameters... };
        vout(LogLevel::Warning, fmtstr.view(), variadic_format_params, true);
    }

    inline void warnln() 
    { 
        warnln(""); 
    }

    void set_log_tag_name(char const*);
    #    endif // MOD_OS_ANDROID

    #    define outln_if(flag, fmt, ...)       \
            do {                               \
                if constexpr (flag)            \
                    outln(fmt, ##__VA_ARGS__); \
            } while (0)

    #    define warnln_if(flag, fmt, ...)       \
            do {                                \
                if constexpr (flag)             \
                    warnln(fmt, ##__VA_ARGS__); \
            } while (0)

    #endif

    /**
     * @param fmtstr 
     * @param newline 
     */
    void vdbg(StringView fmtstr, TypeErasedFormatParams&, bool newline = false);

    /**
     * @tparam Parameters 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void dbg(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
    {
        VariadicFormatParams<AllowDebugOnlyFormatters::Yes, Parameters...> variadic_format_params { parameters... };
        vdbg(fmtstr.view(), variadic_format_params, false);
    }

    /**
     * @tparam Parameters 
     * @param fmtstr 
     * @param parameters 
     */
    template<typename... Parameters>
    void dbgln(CheckedFormatString<Parameters...>&& fmtstr, Parameters const&... parameters)
    {
        VariadicFormatParams<AllowDebugOnlyFormatters::Yes, Parameters...> variadic_format_params { parameters... };
        vdbg(fmtstr.view(), variadic_format_params, true);
    }

    inline void dbgln() 
    { 
        dbgln(""); 
    }

    void set_debug_enabled(bool);
    void set_rich_debug_enabled(bool);

    #ifdef KERNEL
    /**
     * @param fmtstr 
     */
    void vdmesgln(StringView fmtstr, TypeErasedFormatParams&);

    /**
     * @tparam Parameters 
     * @param fmt 
     * @param parameters 
     */
    template<typename... Parameters>
    void dmesgln(CheckedFormatString<Parameters...>&& fmt, Parameters const&... parameters)
    {
        VariadicFormatParams<AllowDebugOnlyFormatters::Yes, Parameters...> variadic_format_params { parameters... };
        vdmesgln(fmt.view(), variadic_format_params);
    }

    /**
     * @param fmtstr 
     */
    void v_critical_dmesgln(StringView fmtstr, TypeErasedFormatParams&);

    /**
     * @tparam Parameters 
     * @param fmt 
     * @param parameters 
     */
    template<typename... Parameters>
    void critical_dmesgln(CheckedFormatString<Parameters...>&& fmt, Parameters const&... parameters)
    {
        VariadicFormatParams<AllowDebugOnlyFormatters::Yes, Parameters...> variadic_format_params { parameters... };
        v_critical_dmesgln(fmt.view(), variadic_format_params);
    }
    #endif

    /**
     * @tparam T 
     */
    template<typename T>
    class FormatIfSupported 
    {
    public:
        /**
         * @brief Construct a new Format If Supported object
         * 
         * @param value 
         */
        explicit FormatIfSupported(T const& value)
            : m_value(value)
        {
        }

        /**
         * @return T const& 
         */
        T const& value() const 
        { 
            return m_value; 
        }

    private:
        T const& m_value;
    }; // class FormatIfSupported 

    /**
     * @tparam T 
     * @tparam Supported 
     */
    template<typename T, bool Supported = false>
    struct __FormatIfSupported : Formatter<StringView> 
    {
        /**
         * @param builder 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, FormatIfSupported<T> const&)
        {
            return Formatter<StringView>::format(builder, "?"sv);
        }
    }; // struct __FormatIfSupported : Formatter<StringView> 

    /**
     * @tparam T 
     */
    template<typename T>
    struct __FormatIfSupported<T, true> : Formatter<T> 
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, FormatIfSupported<T> const& value)
        {
            return Formatter<T>::format(builder, value.value());
        }
    }; // struct __FormatIfSupported<T, true> : Formatter<T> 

    /**
     * @tparam T 
     */
    template<typename T>
    struct Formatter<FormatIfSupported<T>> : __FormatIfSupported<T, HasFormatter<T>> {
    };

    struct FormatString {
    };
    
    template<>
    struct Formatter<FormatString> : Formatter<StringView> 
    {
        /**
         * @tparam Parameters 
         * @param builder 
         * @param fmtstr 
         * @param parameters 
         * @return ErrorOr<void> 
         */
        template<typename... Parameters>
        ErrorOr<void> format(FormatBuilder& builder, StringView fmtstr, Parameters const&... parameters)
        {
            VariadicFormatParams<AllowDebugOnlyFormatters::No, Parameters...> variadic_format_params { parameters... };
            return vformat(builder, fmtstr, variadic_format_params);
        }

        /**
         * @param builder 
         * @param fmtstr 
         * @param params 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> vformat(FormatBuilder& builder, StringView fmtstr, TypeErasedFormatParams& params);
    }; // struct Formatter<FormatString> : Formatter<StringView> 

    /**
     * @tparam  
     */
    template<>
    struct Formatter<Error> : Formatter<FormatString> 
    {
        /**
         * @param builder 
         * @param error 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, Error const& error)
        {
    #if defined(MOD_OS_PRANA) && defined(KERNEL)
            return Formatter<FormatString>::format(builder, "Error(errno={})"sv, error.code());
    #else
            if (error.is_syscall())
                return Formatter<FormatString>::format(builder, "{}: {} (errno={})"sv, error.string_literal(), strerror(error.code()), error.code());
            if (error.is_errno())
                return Formatter<FormatString>::format(builder, "{} (errno={})"sv, strerror(error.code()), error.code());

            return Formatter<FormatString>::format(builder, "{}"sv, error.string_literal());
    #endif
        }
    }; // struct Formatter<Error> : Formatter<FormatString> 

    /**
     * @tparam T 
     * @tparam ErrorType 
     */
    template<typename T, typename ErrorType>
    struct Formatter<ErrorOr<T, ErrorType>> : Formatter<FormatString> 
    {
        /**
         * @return true 
         * @return false 
         */
        static constexpr bool is_debug_only() 
        { 
            return true; 
        }

        /**
         * @param builder 
         * @param error_or 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, ErrorOr<T, ErrorType> const& error_or)
        {
            if (error_or.is_error())
                return Formatter<FormatString>::format(builder, "{}"sv, error_or.error());
            return Formatter<FormatString>::format(builder, "{{{}}}"sv, error_or.value());
        }
    };

    /**
     * @tparam T 
     */
    template<typename T>
    struct Formatter<Optional<T>> : Formatter<FormatString> 
    {
        /**
         * @param builder 
         * @param optional 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, Optional<T> const& optional)
        {
            if (optional.has_value())
                return Formatter<FormatString>::format(builder, "{}"sv, *optional);
            return builder.put_literal("None"sv);
        }
    };

} // namespace Mods

#if USING_MODS_GLOBALLY
#    ifdef KERNEL
using Mods::critical_dmesgln;
using Mods::dmesgln;
#    else
using Mods::out;
using Mods::outln;

using Mods::warn;
using Mods::warnln;
#    endif

using Mods::dbg;
using Mods::dbgln;

using Mods::CheckedFormatString;
using Mods::FormatIfSupported;
using Mods::FormatString;

#    define dbgln_if(flag, fmt, ...)       \
        do {                               \
            if constexpr (flag)            \
                dbgln(fmt, ##__VA_ARGS__); \
        } while (0)

#endif
