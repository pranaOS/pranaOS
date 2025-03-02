/**
 * @file point.h
 * @author Krisna Pranav
 * @brief point
 * @version 6.0
 * @date 2024-10-29
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/format.h>
#include <mods/math.h>
#include <mods/stdlibextra.h>
#include <libgfx/affinetransform.h>
#include <libgfx/forward.h>
#include <libgfx/orientation.h>
#include <libipc/forward.h>
#include <math.h>

namespace Gfx 
{
    /**
     * @tparam T 
     */
    template<typename T>
    class Point 
    {
    public:
        /**
         * @brief Construct a new Point object
         * 
         */
        Point() = default;

        /**
         * @brief Construct a new Point object
         * 
         * @param x 
         * @param y 
         */
        Point(T x, T y)
            : m_x(x)
            , m_y(y)
        {
        }

        /**
         * @brief Construct a new Point object
         * 
         * @tparam U 
         * @param x 
         * @param y 
         */
        template<typename U>
        Point(U x, U y)
            : m_x(x)
            , m_y(y)
        {
        }

        /**
         * @brief Construct a new Point object
         * 
         * @tparam U 
         * @param other 
         */
        template<typename U>
        explicit Point(Point<U> const& other)
            : m_x(other.x())
            , m_y(other.y())
        {
        }

        [[nodiscard]] ALWAYS_INLINE T x() const 
        { 
            return m_x; 
        }

        [[nodiscard]] ALWAYS_INLINE T y() const 
        { 
            return m_y; 
        }

        /**
         * @brief Set the x object
         * 
         * @param x 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void set_x(T x) 
        { 
            m_x = x; 
        }

        /**
         * @brief Set the y object
         * 
         * @param y 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void set_y(T y) 
        { 
            m_y = y; 
        }

        [[nodiscard]] ALWAYS_INLINE bool is_null() const 
        { 
            return !m_x && !m_y; 
        }

        [[nodiscard]] ALWAYS_INLINE bool is_empty() const 
        { 
            return m_x <= 0 && m_y <= 0; 
        }

        /**
         * @param dx 
         * @param dy 
         */
        void translate_by(T dx, T dy)
        {
            m_x += dx;
            m_y += dy;
        }

        /**
         * @param dboth 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void translate_by(T dboth) 
        { 
            translate_by(dboth, dboth); 
        }

        /**
         * @param delta 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void translate_by(Point<T> const& delta) 
        { 
            translate_by(delta.x(), delta.y()); 
        }

        /**
         * @param dx 
         * @param dy 
         */
        void scale_by(T dx, T dy)
        {
            m_x *= dx;
            m_y *= dy;
        }

        /**
         * @param dboth 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void scale_by(T dboth) 
        { 
            scale_by(dboth, dboth); 
        }

        /**
         * @param delta 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void scale_by(Point<T> const& delta) 
        { 
            scale_by(delta.x(), delta.y()); 
        }

        /**
         * @param transform 
         */
        void transform_by(AffineTransform const& transform) 
        { 
            *this = transform.map(*this); 
        }

        /**
         * @param delta 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> translated(Point<T> const& delta) const
        {
            Point<T> point = *this;
            point.translate_by(delta);
            return point;
        }

        /**
         * @param dx 
         * @param dy 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> translated(T dx, T dy) const
        {
            Point<T> point = *this;
            point.translate_by(dx, dy);
            return point;
        }

        /**
         * @param dboth 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> translated(T dboth) const
        {
            Point<T> point = *this;
            point.translate_by(dboth, dboth);
            return point;
        }

        /**
         * @param delta 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> scaled(Point<T> const& delta) const
        {
            Point<T> point = *this;
            point.scale_by(delta);
            return point;
        }   

        /**
         * @param sx 
         * @param sy 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> scaled(T sx, T sy) const
        {
            Point<T> point = *this;
            point.scale_by(sx, sy);
            return point;
        }

        /**
         * @param transform 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> transformed(AffineTransform const& transform) const
        {
            Point<T> point = *this;
            point.transform_by(transform);
            return point;
        }
        
        void constrain(Rect<T> const&);

        /**
         * @param rect 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> constrained(Rect<T> const& rect) const
        {
            Point<T> point = *this;
            point.constrain(rect);
            return point;
        }

        /**
         * @param amount 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> moved_left(T amount) const 
        { 
            return { x() - amount, y() }; 
        }

        /**
         * @param amount 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> moved_right(T amount) const 
        { 
            return { x() + amount, y() }; 
        }

        /**
         * @param amount 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> moved_up(T amount) const 
        { 
            return { x(), y() - amount }; 
        }

        /**
         * @param amount 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> moved_down(T amount) const 
        { 
            return { x(), y() + amount }; 
        }

        /**
         * @tparam U 
         * @param other 
         * @return true 
         * @return false 
         */
        template<class U>
        [[nodiscard]] bool operator==(Point<U> const& other) const
        {
            return x() == other.x() && y() == other.y();
        }

        /**
         * @tparam U 
         * @param other 
         * @return true 
         * @return false 
         */
        template<class U>
        [[nodiscard]] bool operator!=(Point<U> const& other) const
        {
            return !(*this == other);
        }

        /**
         * @param other 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> operator+(Point<T> const& other) const 
        { 
            return { m_x + other.m_x, m_y + other.m_y }; 
        }

        /**
         * @param other 
         * @return Point<T>& 
         */
        Point<T>& operator+=(Point<T> const& other)
        {
            m_x += other.m_x;
            m_y += other.m_y;
            return *this;
        }

        [[nodiscard]] Point<T> operator-() const 
        { 
            return { -m_x, -m_y }; 
        }

        /**
         * @param other 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> operator-(Point<T> const& other) const 
        { 
            return { m_x - other.m_x, m_y - other.m_y }; 
        }

        /**
         * @param other 
         * @return Point<T>& 
         */
        Point<T>& operator-=(Point<T> const& other)
        {
            m_x -= other.m_x;
            m_y -= other.m_y;
            return *this;
        }

        /**
         * @param factor 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> operator*(T factor) const 
        { 
            return { m_x * factor, m_y * factor }; 
        }

        /**
         * @param factor 
         * @return Point<T>& 
         */
        Point<T>& operator*=(T factor)
        {
            m_x *= factor;
            m_y *= factor;
            return *this;
        }

        /**
         * @param factor 
         * @return Point<T> 
         */
        [[nodiscard]] Point<T> operator/(T factor) const 
        { 
            return { m_x / factor, m_y / factor }; 
        }

        /**
         * @param factor 
         * @return Point<T>& 
         */
        Point<T>& operator/=(T factor)
        {
            m_x /= factor;
            m_y /= factor;
            return *this;
        }

        /**
         * @param orientation 
         * @return T 
         */
        [[nodiscard]] T primary_offset_for_orientation(Orientation orientation) const
        {
            return orientation == Orientation::Vertical ? y() : x();
        }

        /**
         * @brief Set the primary offset for orientation object
         * 
         * @param orientation 
         * @param value 
         */
        void set_primary_offset_for_orientation(Orientation orientation, T value)
        {
            if (orientation == Orientation::Vertical) {
                set_y(value);
            } else {
                set_x(value);
            }
        }

        /**
         * @param orientation 
         * @return T 
         */
        [[nodiscard]] T secondary_offset_for_orientation(Orientation orientation) const
        {
            return orientation == Orientation::Vertical ? x() : y();
        }

        /**
         * @brief Set the secondary offset for orientation object
         * 
         * @param orientation 
         * @param value 
         */
        void set_secondary_offset_for_orientation(Orientation orientation, T value)
        {
            if (orientation == Orientation::Vertical) {
                set_x(value);
            } else {
                set_y(value);
            }
        }

        /**
         * @param other 
         * @return T 
         */
        [[nodiscard]] T dx_relative_to(Point<T> const& other) const
        {
            return x() - other.x();
        }

        /**
         * @param other 
         * @return T 
         */
        [[nodiscard]] T dy_relative_to(Point<T> const& other) const
        {
            return y() - other.y();
        }

        /**
         * @param other 
         * @return T 
         */
        [[nodiscard]] T pixels_moved(Point<T> const& other) const
        {
            return max(Mods::abs(dx_relative_to(other)), Mods::abs(dy_relative_to(other)));
        }

        /**
         * @param other 
         * @return float 
         */
        [[nodiscard]] float distance_from(Point<T> const& other) const
        {
            if (*this == other)
                return 0;
            return Mods::hypot<float>(m_x - other.m_x, m_y - other.m_y);
        }

        /**
         * @param other 
         * @return Point 
         */
        [[nodiscard]] Point absolute_relative_distance_to(Point const& other) const
        {
            return { Mods::abs(dx_relative_to(other)), Mods::abs(dy_relative_to(other)) };
        }

        /**
         * @param previous_end_point 
         * @param aspect_ratio 
         * @return Point 
         */
        [[nodiscard]] Point end_point_for_aspect_ratio(Point const& previous_end_point, float aspect_ratio) const;

        /**
         * @tparam U 
         * @return Point<U> 
         */
        template<typename U>
        [[nodiscard]] Point<U> to_type() const
        {
            return Point<U>(*this);
        }

        /**
         * @tparam U 
         * @return Point<U> 
         */
        template<typename U>
        [[nodiscard]] Point<U> to_rounded() const
        {
            return Point<U>(roundf(x()), roundf(y()));
        }

        [[nodiscard]] String to_string() const;

    private:
        T m_x { 0 };
        T m_y { 0 };
    };

    using IntPoint = Point<int>;
    using FloatPoint = Point<float>;

    /**
     * @tparam T 
     * @param p1 
     * @param p2 
     * @param t 
     * @return Point<T> 
     */
    template<typename T>
    inline Point<T> linear_interpolate(Point<T> const& p1, Point<T> const& p2, float t)
    {
        return Point<T> { p1.x() + t * (p2.x() - p1.x()), p1.y() + t * (p2.y() - p1.y()) };
    }

    /**
     * @tparam T 
     * @param p1 
     * @param p2 
     * @param c1 
     * @param t 
     * @return Point<T> 
     */
    template<typename T>
    inline Point<T> quadratic_interpolate(Point<T> const& p1, Point<T> const& p2, Point<T> const& c1, float t)
    {
        return linear_interpolate(linear_interpolate(p1, c1, t), linear_interpolate(c1, p2, t), t);
    }

    /**
     * @tparam T 
     * @param p1 
     * @param p2 
     * @param c1 
     * @param c2 
     * @param t 
     * @return Point<T> 
     */
    template<typename T>
    inline Point<T> cubic_interpolate(Point<T> const& p1, Point<T> const& p2, Point<T> const& c1, Point<T> const& c2, float t)
    {
        return linear_interpolate(quadratic_interpolate(p1, c1, c2, t), quadratic_interpolate(c1, c2, p2, t), t);
    }

}

namespace Mods 
{
    /**
     * @tparam T 
     */
    template<typename T>
    struct Formatter<Gfx::Point<T>> : Formatter<StringView> 
    {
        /**
         * @param builder 
         * @param value 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> format(FormatBuilder& builder, Gfx::Point<T> const& value)
        {
            return Formatter<StringView>::format(builder, value.to_string());
        }
    };
} // namespace Mods

namespace IPC 
{
    bool encode(Encoder&, Gfx::IntPoint const&);
    ErrorOr<void> decode(Decoder&, Gfx::IntPoint&);
} // namespace IPC

/**
 * @tparam T 
 */
template<typename T>
struct Mods::Traits<Gfx::Point<T>> : public Mods::GenericTraits<Gfx::Point<T>> 
{
    static constexpr bool is_trivial() 
    { 
        return false; 
    }

    /**
     * @param point 
     * @return unsigned 
     */
    static unsigned hash(Gfx::Point<T> const& point)
    {
        return pair_int_hash(Mods::Traits<T>::hash(point.x()), Mods::Traits<T>::hash(point.y()));
    }
};
