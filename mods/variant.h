/**
 * @file variant.h
 * @author Krisna Pranav
 * @brief variant
 * @version 6.0
 * @date 2024-08-27
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/array.h>
#include <mods/bitcast.h>
#include <mods/typecast.h>
#include <mods/stdlibextra.h>

namespace Mods::Detail 
{
    /**
     * @tparam T 
     * @tparam IndexType 
     * @tparam InitialIndex 
     * @tparam InTypes 
     */
    template<typename T, typename IndexType, IndexType InitialIndex, typename... InTypes>
    struct VariantIndexOf 
    {
        static_assert(DependentFalse<T, IndexType, InTypes...>, "Invalid Variant Index Found");
    };

    /**
     * @tparam T 
     * @tparam IndexType 
     * @tparam InitialIndex 
     * @tparam InType 
     * @tparam RestOfInTypes 
     */
    template<typename T, typename IndexType, IndexType InitialIndex, typename InType, typename... RestOfInTypes>
    struct VariantIndexOf<T, IndexType, InitialIndex, InType, RestOfInTypes...> 
    {
        consteval IndexType operator()()
        {
            if constexpr (IsSame<T, InType>)
                return InitialIndex;
            else
                return VariantIndexOf<T, IndexType, InitialIndex + 1, RestOfInTypes...> {}();
        }
    };
    
    /**
     * @tparam T 
     * @tparam IndexType 
     * @tparam InitialIndex 
     */
    template<typename T, typename IndexType, IndexType InitialIndex>
    struct VariantIndexOf<T, IndexType, InitialIndex> 
    {
        consteval IndexType operator()() 
        { 
            return InitialIndex; 
        }
    };
    
    /**
     * @tparam T 
     * @tparam IndexType 
     * @tparam Ts 
     * @return consteval 
     */
    template<typename T, typename IndexType, typename... Ts>
    consteval IndexType index_of()
    {
        return VariantIndexOf<T, IndexType, 0, Ts...> {}();
    }

    /**
     * @tparam IndexType 
     * @tparam InitialIndex 
     * @tparam Ts 
     */
    template<typename IndexType, IndexType InitialIndex, typename... Ts>
    struct Variant;

    /**
     * @tparam IndexType 
     * @tparam InitialIndex 
     * @tparam F 
     * @tparam Ts 
     */
    template<typename IndexType, IndexType InitialIndex, typename F, typename... Ts>
    struct Variant<IndexType, InitialIndex, F, Ts...> 
    {
        static constexpr auto current_index = VariantIndexOf<F, IndexType, InitialIndex, F, Ts...> {}();

        /**
         * @param id 
         * @param data 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void delete_(IndexType id, void* data)
        {
            if (id == current_index)
                bit_cast<F*>(data)->~F();
            else
                Variant<IndexType, InitialIndex + 1, Ts...>::delete_(id, data);
        }

        /**
         * @param old_id 
         * @param old_data 
         * @param new_data 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void move_(IndexType old_id, void* old_data, void* new_data)
        {
            if (old_id == current_index)
                new (new_data) F(move(*bit_cast<F*>(old_data)));
            else
                Variant<IndexType, InitialIndex + 1, Ts...>::move_(old_id, old_data, new_data);
        }

        /**
         * @param old_id 
         * @param old_data 
         * @param new_data 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void copy_(IndexType old_id, const void* old_data, void* new_data)
        {
            if (old_id == current_index)
                new (new_data) F(*bit_cast<F const*>(old_data));
            else
                Variant<IndexType, InitialIndex + 1, Ts...>::copy_(old_id, old_data, new_data);
        }
    };

    /**
     * @tparam IndexType 
     * @tparam InitialIndex 
     */
    template<typename IndexType, IndexType InitialIndex>
    struct Variant<IndexType, InitialIndex> 
    {
        ALWAYS_INLINE static void delete_(IndexType, void*) { }
        ALWAYS_INLINE static void move_(IndexType, void*, void*) { }
        ALWAYS_INLINE static void copy_(IndexType, const void*, void*) { }
    };

    template<typename IndexType, typename... Ts>
    struct VisitImpl 
    {
        /**
         * @tparam RT 
         * @tparam T 
         * @tparam I 
         * @tparam Fn 
         * @return true 
         * @return false 
         */
        template<typename RT, typename T, size_t I, typename Fn>
        static constexpr bool has_explicitly_named_overload()
        {
            return requires { (declval<Fn>().*(&Fn::operator()))(declval<T>()); };
        }

        /**
         * @tparam ReturnType 
         * @tparam T 
         * @tparam Visitor 
         * @tparam Is 
         * @return true 
         * @return false 
         */
        template<typename ReturnType, typename T, typename Visitor, auto... Is>
        static constexpr bool should_invoke_const_overload(IndexSequence<Is...>)
        {            
            return ((has_explicitly_named_overload<ReturnType, T, Is, typename Visitor::Types::template Type<Is>>()) || ...);
        }

        /**
         * @tparam Self 
         * @tparam Visitor 
         * @tparam CurrentIndex 
         */
        template<typename Self, typename Visitor, IndexType CurrentIndex = 0>
        ALWAYS_INLINE static constexpr decltype(auto) visit(Self& self, IndexType id, const void* data, Visitor&& visitor) requires(CurrentIndex < sizeof...(Ts))
        {
            using T = typename TypeList<Ts...>::template Type<CurrentIndex>;

            if (id == CurrentIndex) 
            {
                using ReturnType = decltype(visitor(*bit_cast<T*>(data)));
                if constexpr (should_invoke_const_overload<ReturnType, T, Visitor>(MakeIndexSequence<Visitor::Types::size>()))
                    return visitor(*bit_cast<AddConst<T>*>(data));

                return visitor(*bit_cast<CopyConst<Self, T>*>(data));
            }

            if constexpr ((CurrentIndex + 1) < sizeof...(Ts))
                return visit<Self, Visitor, CurrentIndex + 1>(self, id, data, forward<Visitor>(visitor));
            else
                VERIFY_NOT_REACHED();
        }
    };

    struct VariantNoClearTag 
    {
        explicit VariantNoClearTag() = default;
    };

    struct VariantConstructTag 
    {
        explicit VariantConstructTag() = default;
    };

    /**
     * @tparam T 
     * @tparam Base 
     */
    template<typename T, typename Base>
    struct VariantConstructors 
    {
        ALWAYS_INLINE VariantConstructors(T&& t) requires(requires { T(move(t)); })
        {
            internal_cast().clear_without_destruction();
            internal_cast().set(move(t), VariantNoClearTag {});
        }

        ALWAYS_INLINE VariantConstructors(const T& t) requires(requires { T(t); })
        {
            internal_cast().clear_without_destruction();
            internal_cast().set(t, VariantNoClearTag {});
        }

        ALWAYS_INLINE VariantConstructors() = default;

    private:
        [[nodiscard]] ALWAYS_INLINE Base& internal_cast()
        {
            return *reinterpret_cast<Base*>(this);
        }
    };

    struct ParameterPackTag { };

    /**
     * @tparam Ts 
     */
    template<typename... Ts>
    struct ParameterPack : ParameterPackTag { };

    /**
     * @tparam T 
     */
    template<typename T>
    struct Blank { };

    /**
     * @tparam A 
     * @tparam P 
     */
    template<typename A, typename P>
    inline constexpr bool IsTypeInPack = false;

    /**
     * @tparam T 
     * @tparam Ts 
     */
    template<typename T, typename... Ts>
    inline constexpr bool IsTypeInPack<T, ParameterPack<Ts...>> = (IsSame<T, Ts> || ...);

    /**
     * @tparam T 
     * @tparam Qs 
     */
    template<typename T, typename... Qs>


    using BlankIfDuplicate = Conditional<(IsTypeInPack<T, Qs> || ...), Blank<T>, T>;

    template<unsigned I, typename...>
    struct InheritFromUniqueEntries;

    /**
     * @tparam I 
     * @tparam Ts 
     * @tparam Js 
     * @tparam Qs 
     */
    template<unsigned I, typename... Ts, unsigned... Js, typename... Qs>
    struct InheritFromUniqueEntries<I, ParameterPack<Ts...>, IndexSequence<Js...>, Qs...>
        : public BlankIfDuplicate<Ts, Conditional<Js <= I, ParameterPack<>, Qs>...>... {

        using BlankIfDuplicate<Ts, Conditional<Js <= I, ParameterPack<>, Qs>...>::BlankIfDuplicate...;
    };

    template<typename...>
    struct InheritFromPacks;

    template<unsigned... Is, typename... Ps>
    struct InheritFromPacks<IndexSequence<Is...>, Ps...>
        : public InheritFromUniqueEntries<Is, Ps, IndexSequence<Is...>, Ps...>... {

        using InheritFromUniqueEntries<Is, Ps, IndexSequence<Is...>, Ps...>::InheritFromUniqueEntries...;
    };

    template<typename... Ps>
    using MergeAndDeduplicatePacks = InheritFromPacks<MakeIndexSequence<sizeof...(Ps)>, Conditional<IsBaseOf<ParameterPackTag, Ps>, Ps, ParameterPack<Ps>>...>;

} // namespace Mods::Detail

namespace Mods
{

    struct Empty { };

    template<typename... Ts>
    struct Variant
        : public Detail::MergeAndDeduplicatePacks<Detail::VariantConstructors<Ts, Variant<Ts...>>...> 
    {
    private:
        using IndexType = Conditional<sizeof...(Ts) < 255, u8, size_t>; 

        static constexpr IndexType invalid_index = sizeof...(Ts);

        template<typename T>
        static constexpr IndexType index_of() 
        { 
            return Detail::index_of<T, IndexType, Ts...>(); 
        }

    public:

        /**
         * @tparam T 
         * @return true 
         * @return false 
         */
        template<typename T>
        static constexpr bool can_contain()
        {
            return index_of<T>() != invalid_index;
        }

        /**
         * @tparam NewTs 
         */
        template<typename... NewTs>
        Variant(Variant<NewTs...>&& old) requires((can_contain<NewTs>() && ...))
            : Variant(move(old).template downcast<Ts...>())
        { }

        /**
         * @tparam NewTs 
         */
        template<typename... NewTs>
        Variant(const Variant<NewTs...>& old) requires((can_contain<NewTs>() && ...))
            : Variant(old.template downcast<Ts...>())
        { }

        /**
         * @tparam NewTs 
         */
        template<typename... NewTs>
        friend struct Variant;

        Variant() requires(!can_contain<Empty>()) = delete;

        Variant() requires(can_contain<Empty>())
            : Variant(Empty())
        { }

    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
        Variant(const Variant&) requires(!(IsCopyConstructible<Ts> && ...)) = delete;

        Variant(const Variant&) = default;

        Variant(Variant&&) requires(!(IsMoveConstructible<Ts> && ...)) = delete;

        Variant(Variant&&) = default;

        ~Variant() requires(!(IsDestructible<Ts> && ...)) = delete;

        ~Variant() = default;

        Variant& operator=(const Variant&) requires(!(IsCopyConstructible<Ts> && ...) || !(IsDestructible<Ts> && ...)) = delete;

        Variant& operator=(const Variant&) = default;

        Variant& operator=(Variant&&) requires(!(IsMoveConstructible<Ts> && ...) || !(IsDestructible<Ts> && ...)) = delete;
        Variant& operator=(Variant&&) = default;
    #endif

        ALWAYS_INLINE Variant(const Variant& old)

    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
            requires(!(IsTriviallyCopyConstructible<Ts> && ...))
    #endif
            : Detail::MergeAndDeduplicatePacks<Detail::VariantConstructors<Ts, Variant<Ts...>>...>()
            , m_data {}
            , m_index(old.m_index)
        {
            Helper::copy_(old.m_index, old.m_data, m_data);
        }

        ALWAYS_INLINE Variant(Variant&& old)

    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
            requires(!(IsTriviallyMoveConstructible<Ts> && ...))
    #endif
            : Detail::MergeAndDeduplicatePacks<Detail::VariantConstructors<Ts, Variant<Ts...>>...>()
            , m_index(old.m_index)
        {
            Helper::move_(old.m_index, old.m_data, m_data);
        }

        ALWAYS_INLINE ~Variant()

    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
            requires(!(IsTriviallyDestructible<Ts> && ...))
    #endif
        {
            Helper::delete_(m_index, m_data);
        }

        ALWAYS_INLINE Variant& operator=(const Variant& other)

    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
            requires(!(IsTriviallyCopyConstructible<Ts> && ...) || !(IsTriviallyDestructible<Ts> && ...))
    #endif
        {
            if (this != &other) {
                if constexpr (!(IsTriviallyDestructible<Ts> && ...)) {
                    Helper::delete_(m_index, m_data);
                }
                m_index = other.m_index;
                Helper::copy_(other.m_index, other.m_data, m_data);
            }
            return *this;
        }

        ALWAYS_INLINE Variant& operator=(Variant&& other)

    #ifdef MODS_HAS_CONDITIONALLY_TRIVIAL
            requires(!(IsTriviallyMoveConstructible<Ts> && ...) || !(IsTriviallyDestructible<Ts> && ...))
    #endif
        {
            if (this != &other) {
                if constexpr (!(IsTriviallyDestructible<Ts> && ...)) {
                    Helper::delete_(m_index, m_data);
                }
                m_index = other.m_index;
                Helper::move_(other.m_index, other.m_data, m_data);
            }
            return *this;
        }

        using Detail::MergeAndDeduplicatePacks<Detail::VariantConstructors<Ts, Variant<Ts...>>...>::MergeAndDeduplicatePacks;
        
        /**
         * @tparam T 
         * @tparam StrippedT 
         */
        template<typename T, typename StrippedT = RemoveCVReference<T>>
        void set(T&& t) requires(can_contain<StrippedT>() && requires { StrippedT(forward<T>(t)); })
        {
            constexpr auto new_index = index_of<StrippedT>();
            Helper::delete_(m_index, m_data);
            new (m_data) StrippedT(forward<T>(t));
            m_index = new_index;
        }

        /**
         * @tparam T 
         * @tparam StrippedT 
         */
        template<typename T, typename StrippedT = RemoveCVReference<T>>
        void set(T&& t, Detail::VariantNoClearTag) requires(can_contain<StrippedT>() && requires { StrippedT(forward<T>(t)); })
        {
            constexpr auto new_index = index_of<StrippedT>();
            new (m_data) StrippedT(forward<T>(t));
            m_index = new_index;
        }

        /**
         * @tparam T 
         */
        template<typename T>
        T* get_pointer() requires(can_contain<T>())
        {
            if (index_of<T>() == m_index)
                return bit_cast<T*>(&m_data);
            return nullptr;
        }

        /**
         * @tparam T 
         */
        template<typename T>
        T& get() requires(can_contain<T>())
        {
            VERIFY(has<T>());
            return *bit_cast<T*>(&m_data);
        }

        /**
         * @tparam T 
         */
        template<typename T>
        const T* get_pointer() const requires(can_contain<T>())
        {
            if (index_of<T>() == m_index)
                return bit_cast<const T*>(&m_data);
            return nullptr;
        }

        /**
         * @tparam T 
         */
        template<typename T>
        const T& get() const requires(can_contain<T>())
        {
            VERIFY(has<T>());
            return *bit_cast<const T*>(&m_data);
        }

        /**
         * @tparam T 
         */
        template<typename T>
        [[nodiscard]] bool has() const requires(can_contain<T>())
        {
            return index_of<T>() == m_index;
        }

        /**
         * @tparam Fs 
         * @param functions 
         * @return ALWAYS_INLINE 
         */
        template<typename... Fs>
        ALWAYS_INLINE decltype(auto) visit(Fs&&... functions)
        {
            Visitor<Fs...> visitor { forward<Fs>(functions)... };
            return VisitHelper::visit(*this, m_index, m_data, move(visitor));
        }

        /**
         * @tparam Fs 
         * @param functions 
         * @return ALWAYS_INLINE 
         */
        template<typename... Fs>
        ALWAYS_INLINE decltype(auto) visit(Fs&&... functions) const
        {
            Visitor<Fs...> visitor { forward<Fs>(functions)... };
            return VisitHelper::visit(*this, m_index, m_data, move(visitor));
        }

        /**
         * @tparam NewTs 
         * @return Variant<NewTs...> 
         */
        template<typename... NewTs>
        Variant<NewTs...> downcast() &&
        {
            Variant<NewTs...> instance { Variant<NewTs...>::invalid_index, Detail::VariantConstructTag {} };
            visit([&](auto& value) {
                if constexpr (Variant<NewTs...>::template can_contain<RemoveCVReference<decltype(value)>>())
                    instance.set(move(value), Detail::VariantNoClearTag {});
            });
            VERIFY(instance.m_index != instance.invalid_index);
            return instance;
        }

        /**
         * @tparam NewTs 
         * @return Variant<NewTs...> 
         */
        template<typename... NewTs>
        Variant<NewTs...> downcast() const&
        {
            Variant<NewTs...> instance { Variant<NewTs...>::invalid_index, Detail::VariantConstructTag {} };
            visit([&](const auto& value) {
                if constexpr (Variant<NewTs...>::template can_contain<RemoveCVReference<decltype(value)>>())
                    instance.set(value, Detail::VariantNoClearTag {});
            });
            VERIFY(instance.m_index != instance.invalid_index);
            return instance;
        }

    private:
        static constexpr auto data_size = Detail::integer_sequence_generate_array<size_t>(0, IntegerSequence<size_t, sizeof(Ts)...>()).max();

        static constexpr auto data_alignment = Detail::integer_sequence_generate_array<size_t>(0, IntegerSequence<size_t, alignof(Ts)...>()).max();

        using Helper = Detail::Variant<IndexType, 0, Ts...>;
        using VisitHelper = Detail::VisitImpl<IndexType, Ts...>;

        /**
         * @tparam T_ 
         * @tparam U_ 
         */
        template<typename T_, typename U_>
        friend struct Detail::VariantConstructors;

        /**
         * @param index 
         */
        explicit Variant(IndexType index, Detail::VariantConstructTag)
            : Detail::MergeAndDeduplicatePacks<Detail::VariantConstructors<Ts, Variant<Ts...>>...>()
            , m_index(index)
        { }

        ALWAYS_INLINE void clear_without_destruction()
        {
            __builtin_memset(m_data, 0, data_size);
            m_index = invalid_index;
        }

        /**
         * @tparam Fs 
         */
        template<typename... Fs>
        struct Visitor : Fs... {
            using Types = TypeList<Fs...>;

            Visitor(Fs&&... args)
                : Fs(forward<Fs>(args))...
            {
            }

            using Fs::operator()...;
        };

        alignas(data_alignment) u8 m_data[data_size];
        IndexType m_index;
    };

} // namespace Mods

using Mods::Empty;
using Mods::Variant;
