/**
 * @file affinetransform.h
 * @author Krisna Pranav
 * @brief Affine Transform
 * @version 6.0
 * @date 2024-10-29
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/format.h>
#include <mods/forward.h>
#include <libgfx/forward.h>

namespace Gfx 
{
    class AffineTransform 
    {
    public:
        /**
         * @brief Construct a new Affine Transform object
         * 
         */
        AffineTransform()
            : m_values { 1, 0, 0, 1, 0, 0 }
        {
        }

        /**
         * @brief Construct a new Affine Transform object
         * 
         * @param a 
         * @param b 
         * @param c 
         * @param d 
         * @param e 
         * @param f 
         */
        AffineTransform(float a, float b, float c, float d, float e, float f)
            : m_values { a, b, c, d, e, f }
        {
        }

        bool is_identity() const;

        /**
         * @param unmapped_x 
         * @param unmapped_y 
         * @param mapped_x 
         * @param mapped_y 
         */
        void map(float unmapped_x, float unmapped_y, float& mapped_x, float& mapped_y) const;

        /**
         * @tparam T 
         * @return Point<T> 
         */
        template<typename T>
        Point<T> map(const Point<T>&) const;

        /**
         * @tparam T 
         * @return Size<T> 
         */
        template<typename T>
        Size<T> map(const Size<T>&) const;

        /**
         * @tparam T 
         * @return Rect<T> 
         */
        template<typename T>
        Rect<T> map(const Rect<T>&) const;

        [[nodiscard]] ALWAYS_INLINE float a() const 
        { 
            return m_values[0]; 
        }

        [[nodiscard]] ALWAYS_INLINE float b() const 
        { 
            return m_values[1]; 
        }

        [[nodiscard]] ALWAYS_INLINE float c() const 
        { 
            return m_values[2]; 
        }

        [[nodiscard]] ALWAYS_INLINE float d() const 
        { 
            return m_values[3]; 
        }

        [[nodiscard]] ALWAYS_INLINE float e() const 
        { 
            return m_values[4]; 
        }

        [[nodiscard]] ALWAYS_INLINE float f() const 
        { 
            return m_values[5]; 
        }

        [[nodiscard]] float x_scale() const;
        [[nodiscard]] float y_scale() const;
        [[nodiscard]] FloatPoint scale() const;
        [[nodiscard]] float x_translation() const;
        [[nodiscard]] float y_translation() const;
        [[nodiscard]] FloatPoint translation() const;

        /**
         * @param sx 
         * @param sy 
         * @return AffineTransform& 
         */
        AffineTransform& scale(float sx, float sy);

        /**
         * @param s 
         * @return AffineTransform& 
         */
        AffineTransform& scale(const FloatPoint& s);

        /**
         * @brief Set the scale object
         * 
         * @param sx 
         * @param sy 
         * @return AffineTransform& 
         */
        AffineTransform& set_scale(float sx, float sy);

        /**
         * @brief Set the scale object
         * 
         * @param s 
         * @return AffineTransform& 
         */
        AffineTransform& set_scale(const FloatPoint& s);

        /**
         * @param tx 
         * @param ty 
         * @return AffineTransform& 
         */
        AffineTransform& translate(float tx, float ty);

        /**
         * @param t 
         * @return AffineTransform& 
         */
        AffineTransform& translate(const FloatPoint& t);

        /**
         * @brief Set the translation object
         * 
         * @param tx 
         * @param ty 
         * @return AffineTransform& 
         */
        AffineTransform& set_translation(float tx, float ty);

        /**
         * @brief Set the translation object
         * 
         * @param t 
         * @return AffineTransform& 
         */
        AffineTransform& set_translation(const FloatPoint& t);

        /**
         * @return AffineTransform& 
         */
        AffineTransform& rotate_radians(float);

        /**
         * @return AffineTransform& 
         */
        AffineTransform& multiply(const AffineTransform&);

    private:
        float m_values[6] { 0 };
    }; // class AffineTransform
} // namespace Gfx

template<>
struct Mods::Formatter<Gfx::AffineTransform> : Formatter<FormatString> 
{
    /**
     * @param builder 
     * @param value 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> format(FormatBuilder& builder, Gfx::AffineTransform const& value)
    {
        return Formatter<FormatString>::format(builder, "[{} {} {} {} {} {}]", value.a(), value.b(), value.c(), value.d(), value.e(), value.f());
    }
};
