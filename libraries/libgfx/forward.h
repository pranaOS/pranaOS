/**
 * @file forward.h
 * @author Krisna Pranav
 * @brief Forward
 * @version 6.0
 * @date 2024-10-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Gfx 
{
    class Bitmap;
    class CharacterBitmap;
    class Color;
    class DisjointRectSet;
    class Emoji;
    class Font;
    class GlyphBitmap;
    class ImageDecoder;

    struct FontMetrics;

    /**
     * @tparam T 
     */
    template<typename T>
    class Line;

    class Painter;
    class Palette;
    class PaletteImpl;
    class Path;
    class ShareableBitmap;
    class StylePainter;
    struct SystemTheme;
    class Triangle;

    /**
     * @tparam T 
     */
    template<typename T>
    class Point;

    /**
     * @tparam T 
     */
    template<typename T>
    class Size;

    /**
     * @tparam T 
     */
    template<typename T>
    class Rect;

    using IntLine = Line<int>;
    using FloatLine = Line<float>;

    using IntRect = Rect<int>;
    using FloatRect = Rect<float>;

    using IntPoint = Point<int>;
    using FloatPoint = Point<float>;

    using IntSize = Size<int>;
    using FloatSize = Size<float>;

    enum class BitmapFormat;
    enum class ColorRole;
    enum class TextAlignment;
} // namespace Gfx

using Gfx::Color;
