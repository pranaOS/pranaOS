/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Array.h>
#include <base/BitCast.h>
#include <base/StdLibExtras.h>
#include <base/TypeList.h>

namespace Base::Detail {

template<typename T, typename IndexType, IndexType InitialIndex, typename... InTypes>
struct VariantIndexOf {
    static_assert(DependentFalse<T, IndexType, InTypes...>, "Invalid VariantIndex instantiated");
};

template<typename T, typename IndexType, IndexType InitialIndex, typename InType, typename... RestOfInTypes>
struct VariantIndexOf<T, IndexType, InitialIndex, InType, RestOfInTypes...> {
    consteval IndexType operator()()
    {
        if constexpr (IsSame<T, InType>)
            return InitialIndex;
        else
            return VariantIndexOf<T, IndexType, InitialIndex + 1, RestOfInTypes...> {}();
    }
};

template<typename T, typename IndexType, IndexType InitialIndex>
struct VariantIndexOf<T, IndexType, InitialIndex> {
    consteval IndexType operator()() { return InitialIndex; }
};

template<typename T, typename IndexType, typename... Ts>
consteval IndexType index_of()
{
    return VariantIndexOf<T, IndexType, 0, Ts...> {}();
}

template<typename IndexType, IndexType InitialIndex, typename... Ts>
struct Variant;

template<typename IndexType, IndexType InitialIndex, typename F, typename... Ts>
struct Variant<IndexType, InitialIndex, F, Ts...> {
    static constexpr auto current_index = VariantIndexOf<F, IndexType, InitialIndex, F, Ts...> {}();
    ALWAYS_INLINE static void delete_(IndexType id, void* data)
    {
        if (id == current_index)
            bit_cast<F*>(data)->~F();
        else
            Variant<IndexType, InitialIndex + 1, Ts...>::delete_(id, data);
    }

    ALWAYS_INLINE static void move_(IndexType old_id, void* old_data, void* new_data)
    {
        if (old_id == current_index)
            new (new_data) F(move(*bit_cast<F*>(old_data)));
        else
            Variant<IndexType, InitialIndex + 1, Ts...>::move_(old_id, old_data, new_data);
    }

    ALWAYS_INLINE static void copy_(IndexType old_id, const void* old_data, void* new_data)
    {
        if (old_id == current_index)
            new (new_data) F(*bit_cast<F const*>(old_data));
        else
            Variant<IndexType, InitialIndex + 1, Ts...>::copy_(old_id, old_data, new_data);
    }
};

template<typename IndexType, IndexType InitialIndex>
struct Variant<IndexType, InitialIndex> {
    ALWAYS_INLINE static void delete_(IndexType, void*) { }
    ALWAYS_INLINE static void move_(IndexType, void*, void*) { }
    ALWAYS_INLINE static void copy_(IndexType, const void*, void*) { }
};

template<typename IndexType, typename... Ts>
struct VisitImpl {
    template<typename Visitor, IndexType CurrentIndex = 0>
    ALWAYS_INLINE static constexpr decltype(auto) visit(IndexType id, const void* data, Visitor&& visitor) requires(CurrentIndex < sizeof...(Ts))
    {
        using T = typename TypeList<Ts...>::template Type<CurrentIndex>;

        if (id == CurrentIndex)
            return visitor(*bit_cast<T*>(data));

        if constexpr ((CurrentIndex + 1) < sizeof...(Ts))
            return visit<Visitor, CurrentIndex + 1>(id, data, forward<Visitor>(visitor));
        else
            VERIFY_NOT_REACHED();
    }
};

struct VariantNoClearTag {
    explicit VariantNoClearTag() = default;
};
struct VariantConstructTag {
    explicit VariantConstructTag() = default;
};

template<typename T, typename Base>
struct VariantConstructors {
    ALWAYS_INLINE VariantConstructors(T&& t)
    {
        internal_cast().clear_without_destruction();
        internal_cast().set(move(t), VariantNoClearTag {});
    }

    ALWAYS_INLINE VariantConstructors(const T& t)
    {
        internal_cast().clear_without_destruction();
        internal_cast().set(t, VariantNoClearTag {});
    }

    ALWAYS_INLINE VariantConstructors() { }

private:
    [[nodiscard]] ALWAYS_INLINE Base& internal_cast()
    {
        return *reinterpret_cast<Base*>(this);
    }
};


struct ParameterPackTag {
};


}