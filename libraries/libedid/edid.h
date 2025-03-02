/**
 * @file edid.h
 * @author Krisna Pranav
 * @brief EDID
 * @version 6.0
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/byte_buffer.h>
#include <mods/bytereader.h>
#include <mods/endian.h>
#include <mods/error.h>
#include <mods/fixedpoint.h>
#include <mods/forward.h>
#include <mods/span.h>
#include <mods/vector.h>
#include <libedid/dmt.h>
#include <libedid/vic.h>

#ifdef KERNEL
#    include <kernel/kstring.h>
#else
#    include <mods/string.h>
#endif

namespace EDID 
{

    namespace Definitions 
    {
        struct EDID;
        struct DetailedTiming;
        struct DisplayDescriptor;
        struct ExtensionBlock;
    } // namespace Definitions

    class Parser final 
    {
        friend class CEA861ExtensionBlock;

    public:
        static constexpr size_t BufferSize = 128;
        using RawBytes = unsigned char[BufferSize];

    protected:
        class DisplayFeatures 
        {
        public:
            bool supports_standby() const 
            { 
                return (m_features & (1 << 7)) != 0; 
            }

            bool supports_suspend() const 
            { 
                return (m_features & (1 << 6)) != 0; 
            }

            bool supports_off() const 
            { 
                return (m_features & (1 << 5)) != 0; 
            }

            bool preferred_timing_mode_includes_pixel_format_and_refresh_rate() const
            {
                if (m_edid_revision < 4)
                    return true; 
                return (m_features & (1 << 1)) != 0;
            }

            bool srgb_is_default_color_space() const 
            { 
                return (m_features & (1 << 2)) != 0; 
            }

            enum class Frequency : u8 
            {
                Continuous,
                NonContinuous,
                DefaultGTF,
                VESA_DMT
            }; // enum class Frequency : u8

            /**
             * @return Frequency 
             */
            Frequency frequency() const
            {
                if (m_edid_revision < 4)
                    return ((m_features & 1) != 0) ? Frequency::DefaultGTF : Frequency::VESA_DMT;
                return ((m_features & 1) != 0) ? Frequency::Continuous : Frequency::NonContinuous;
            }

        protected:
            /**
             * @brief Construct a new Display Features object
             * 
             * @param features 
             * @param edid_revision 
             */
            DisplayFeatures(u8 features, u8 edid_revision)
                : m_features(features)
                , m_edid_revision(edid_revision)
            {
            }

            u8 m_features { 0 };
            u8 m_edid_revision { 0 };
        }; // class DisplayFeatures 

    public:
        static ErrorOr<Parser> from_bytes(ReadonlyBytes);
        static ErrorOr<Parser> from_bytes(ByteBuffer&&);

    #ifndef KERNEL
        static ErrorOr<Parser> from_framebuffer_device(int, size_t);
        static ErrorOr<Parser> from_framebuffer_device(String const&, size_t);
    #endif

        StringView legacy_manufacturer_id() const;
    #ifndef KERNEL
        String manufacturer_name() const;
    #endif

        u16 product_code() const;
        u32 serial_number() const;

        class DigitalDisplayFeatures final : public DisplayFeatures 
        {
            friend class Parser;

        public:
            enum class SupportedColorEncodings : u8 
            {
                RGB444,
                RGB444_YCrCb444,
                RGB444_YCrCb422,
                RGB444_YCrCb444_YCrCb422
            }; // enum class SupportedColorEncodings : u8 

            /**
             * @return SupportedColorEncodings 
             */
            SupportedColorEncodings supported_color_encodings() const 
            { 
                return (SupportedColorEncodings)((m_features >> 3) & 3); 
            }

        private:
            /**
             * @brief Construct a new Digital Display Features object
             * 
             * @param features 
             * @param edid_revision 
             */
            DigitalDisplayFeatures(u8 features, u8 edid_revision)
                : DisplayFeatures(features, edid_revision)
            {
            }
        }; // class DigitalDisplayFeatures final : public DisplayFeatures 

        class AnalogDisplayFeatures final : public DisplayFeatures 
        {
            friend class Parser;

        public:
            enum class DisplayColorType : u8 
            {
                MonochromeOrGrayscale,
                RGB,
                NonRGB,
                Undefined
            }; // enum class DisplayColorType : u8 

            /**
             * @return DisplayColorType 
             */
            DisplayColorType display_color_type() const 
            { 
                return (DisplayColorType)((m_features >> 3) & 3); 
            }

        private:
            /**
             * @brief Construct a new Analog Display Features object
             * 
             * @param features 
             * @param edid_revision 
             */
            AnalogDisplayFeatures(u8 features, u8 edid_revision)
                : DisplayFeatures(features, edid_revision)
            {
            }
        }; /// class AnalogDisplayFeatures final : public DisplayFeatures 

        class DigitalDisplay final 
        {
            friend class Parser;

        public:
            enum class ColorBitDepth : u8 
            {
                Undefined = 0,
                BPP_6,
                BPP_8,
                BPP_10,
                BPP_12,
                BPP_14,
                BPP_16,
                Reserved
            }; // enum class ColorBitDepth : u8 

            enum class SupportedInterface : u8 
            {
                Undefined = 0,
                DVI,
                HDMI_A,
                HDMI_B,
                MDDI,
                DisplayPort,
                Reserved
            }; // enum class SupportedInterface : u8 

            /**
             * @return ColorBitDepth 
             */
            ColorBitDepth color_bit_depth() const 
            { 
                return (ColorBitDepth)((m_video_input_definition >> 4) & 7); 
            }

            /**
             * @return SupportedInterface 
             */
            SupportedInterface supported_interface() const 
            { 
                return ((m_video_input_definition & 0xf) <= 5) ? (SupportedInterface)(m_video_input_definition & 0xf) : SupportedInterface::Reserved; 
            }

            DigitalDisplayFeatures const& features() 
            { 
                return m_features; 
            }

        private:
            /**
             * @brief Construct a new Digital Display object
             * 
             * @param video_input_definition 
             * @param features 
             * @param edid_revision 
             */
            DigitalDisplay(u8 video_input_definition, u8 features, u8 edid_revision)
                : m_video_input_definition(video_input_definition)
                , m_features(features, edid_revision)
            {
            }

            u8 m_video_input_definition { 0 };
            DigitalDisplayFeatures m_features;
        }; // class DigitalDisplay final 

        Optional<DigitalDisplay> digital_display() const;

        class AnalogDisplay final 
        {
            friend class Parser;

        public:
            bool separate_sync_h_and_v_supported() const 
            { 
                return (m_video_input_definition & (1 << 3)) != 0; 
            }

        private:
            /**
             * @brief Construct a new Analog Display object
             * 
             * @param video_input_definition 
             * @param features 
             * @param edid_revision 
             */
            AnalogDisplay(u8 video_input_definition, u8 features, u8 edid_revision)
                : m_video_input_definition(video_input_definition)
                , m_features(features, edid_revision)
            {
            }

            u8 m_video_input_definition { 0 };
            AnalogDisplayFeatures m_features;
        }; // class AnalogDisplay final 

        Optional<AnalogDisplay> analog_display() const;

        class ScreenSize final 
        {
            friend class Parser;

        public:
            /**
             * @return unsigned 
             */
            unsigned horizontal_cm() const 
            { 
                return m_horizontal_cm; 
            }

            unsigned vertical_cm() const 
            { 
                return m_vertical_cm; 
            }

        private:
            /**
             * @brief Construct a new Screen Size object
             * 
             * @param horizontal_cm 
             * @param vertical_cm 
             */
            ScreenSize(u8 horizontal_cm, u8 vertical_cm)
                : m_horizontal_cm(horizontal_cm)
                , m_vertical_cm(vertical_cm)
            {
            }

            u8 m_horizontal_cm { 0 };
            u8 m_vertical_cm { 0 };
        }; // class ScreenSize final 

        Optional<ScreenSize> screen_size() const;

        class ScreenAspectRatio final 
        {
            friend class Parser;

        public:
            enum class Orientation 
            {
                Landscape,
                Portrait
            }; // enum class Orientation 

            Orientation orientation() const 
            { 
                return m_orientation; 
            }

            auto ratio() const 
            { 
                return m_ratio; 
            }

        private:
            /**
             * @brief Construct a new Screen Aspect Ratio object
             * 
             * @param orientation 
             * @param ratio 
             */
            ScreenAspectRatio(Orientation orientation, FixedPoint<16> ratio)
                : m_orientation(orientation)
                , m_ratio(ratio)
            {
            }

            Orientation m_orientation { Orientation::Landscape };
            FixedPoint<16> m_ratio {};
        }; // class ScreenAspectRatio final 

        Optional<ScreenAspectRatio> aspect_ratio() const;

        Optional<FixedPoint<16>> gamma() const;

        class EstablishedTiming final 
        {
            friend class Parser;

        public:
            enum class Source 
            {
                IBM,
                Apple,
                VESA,
                Manufacturer
            }; // enum class Source 

            ALWAYS_INLINE Source source() const 
            { 
                return m_source; 
            }

            ALWAYS_INLINE unsigned width() const 
            { 
                return m_width;
            };

            ALWAYS_INLINE unsigned height() const 
            { 
                return m_height; 
            }

            ALWAYS_INLINE unsigned refresh_rate() const
            {
                if (m_source == Source::Manufacturer)
                    return 0;
                return m_refresh_rate_or_manufacturer_specific;
            }

            ALWAYS_INLINE u8 manufacturer_specific() const
            {
                VERIFY(m_source == Source::Manufacturer);
                return m_refresh_rate_or_manufacturer_specific;
            }

            ALWAYS_INLINE u8 dmt_id() const 
            { 
                return m_dmt_id; 
            }

        private:
            /**
             * @param source
             * @param width
             * @param height
             * @param refresh_rate_of_manufacturer
             * @param dmt_id
             * 
             */
            constexpr EstablishedTiming(Source source, u16 width, u16 height, u8 refresh_rate_or_manufacturer_specific, u8 dmt_id = 0)
                : m_source(source)
                , m_width(width)
                , m_height(height)
                , m_refresh_rate_or_manufacturer_specific(refresh_rate_or_manufacturer_specific)
                , m_dmt_id(dmt_id)
            {
            }

            Source m_source { Source::IBM };
            u16 m_width { 0 };
            u16 m_height { 0 };
            u8 m_refresh_rate_or_manufacturer_specific { 0 };
            u8 m_dmt_id { 0 };
        }; // class EstablishedTiming final 

        ErrorOr<IterationDecision> for_each_established_timing(Function<IterationDecision(EstablishedTiming const&)>) const;

        class StandardTiming final 
        {
            friend class Parser;

        public:
            enum class AspectRatio 
            {
                AR_16_10,
                AR_4_3,
                AR_5_4,
                AR_16_9
            }; // enum class AspectRatio 

            unsigned width() const 
            { 
                return m_width; 
            }

            unsigned height() const 
            { 
                return m_height; 
            }

            unsigned refresh_rate() const 
            { 
                return m_refresh_rate; 
            }

            AspectRatio aspect_ratio() const 
            { 
                return m_aspect_ratio; 
            }
            
            u8 dmt_id() const 
            { 
                return m_dmt_id; 
            }

        private:
            /**
             * @return width
             * @return height
             * @return refresh_rate
             * @return aspect_ratio
             * @return dmt-id
             * 
             */
            constexpr StandardTiming(u16 width, u16 height, u8 refresh_rate, AspectRatio aspect_ratio, u8 dmt_id)
                : m_width(width)
                , m_height(height)
                , m_refresh_rate(refresh_rate)
                , m_aspect_ratio(aspect_ratio)
                , m_dmt_id(dmt_id)
            {
            }

            u16 m_width { 0 };
            u16 m_height { 0 };
            u8 m_refresh_rate { 0 };
            AspectRatio m_aspect_ratio { AspectRatio::AR_16_10 };
            u8 m_dmt_id { 0 };
        }; // class StandardTiming final 

        ErrorOr<IterationDecision> for_each_standard_timing(Function<IterationDecision(StandardTiming const&)>) const;

        class DetailedTiming final 
        {
            friend class Parser;
            friend class CEA861ExtensionBlock;

        public:
            u32 pixel_clock_khz() const;

            u16 horizontal_addressable_pixels() const;

            u16 horizontal_blanking_pixels() const;

            u16 vertical_addressable_lines() const;

            u16 vertical_blanking_lines() const;

            u16 horizontal_front_porch_pixels() const;
            
            ALWAYS_INLINE u16 horizontal_back_porch_pixels() const 
            { 
                return horizontal_blanking_pixels() - horizontal_sync_pulse_width_pixels() - horizontal_front_porch_pixels(); 
            }

            u16 horizontal_sync_pulse_width_pixels() const;

            u16 vertical_front_porch_lines() const;

            ALWAYS_INLINE u16 vertical_back_porch_lines() const 
            { 
                return vertical_blanking_lines() - vertical_sync_pulse_width_lines() - vertical_front_porch_lines(); 
            }

            u16 vertical_sync_pulse_width_lines() const;

            u16 horizontal_image_size_mm() const;

            u16 vertical_image_size_mm() const;

            u8 horizontal_right_or_left_border_pixels() const;
            
            u8 vertical_top_or_bottom_border_lines() const;

            bool is_interlaced() const;
            FixedPoint<16, u32> refresh_rate() const;

        private:
            /**
             * @brief Construct a new Detailed Timing object
             * 
             * @param edid 
             * @param detailed_timings 
             */
            DetailedTiming(Parser const& edid, Definitions::DetailedTiming const* detailed_timings)
                : m_edid(edid)
                , m_detailed_timings(*detailed_timings)
            {
            }

            u16 vertical_addressable_lines_raw() const;

            Parser const& m_edid;
            Definitions::DetailedTiming const& m_detailed_timings;
        }; // class DetailedTiming final 

        ErrorOr<IterationDecision> for_each_detailed_timing(Function<IterationDecision(DetailedTiming const&, unsigned)>) const;
        Optional<DetailedTiming> detailed_timing(size_t) const;

    #ifndef KERNEL
        String display_product_name() const;
        String display_product_serial_number() const;
    #endif

        ErrorOr<IterationDecision> for_each_short_video_descriptor(Function<IterationDecision(unsigned, bool, VIC::Details const&)>) const;

        class CoordinatedVideoTiming final 
        {
            friend class Parser;

        public:
            enum class AspectRatio : u8 
            {
                AR_4_3 = 0,
                AR_16_9 = 1,
                AR_16_10 = 2,
                AR_15_9 = 3
            }; // enum class AspectRatio : u8 

            u16 horizontal_addressable_pixels() const;

            u16 vertical_addressable_lines() const;

            AspectRatio aspect_ratio() const;
            
            u16 preferred_refresh_rate();

            ALWAYS_INLINE DMT::CVT cvt_code() const 
            { 
                return m_cvt; 
            }

        private:
            /**
             * @brief Construct a new Coordinated Video Timing object
             * 
             * @param cvt 
             */
            CoordinatedVideoTiming(DMT::CVT const& cvt)
                : m_cvt(cvt)
            {
            }

            DMT::CVT m_cvt;
        }; // class CoordinatedVideoTiming final 

        ErrorOr<IterationDecision> for_each_coordinated_video_timing(Function<IterationDecision(CoordinatedVideoTiming const&)>) const;

        ErrorOr<IterationDecision> for_each_extension_block(Function<IterationDecision(unsigned, u8, u8, ReadonlyBytes)>) const;

        struct SupportedResolution 
        {
            unsigned width { 0 };
            unsigned height { 0 };

            struct RefreshRate 
            {
                FixedPoint<16, u32> rate;
                bool preferred { false };

                /**
                 * @param rhs 
                 * @return true 
                 * @return false 
                 */
                bool operator<(RefreshRate const& rhs) const 
                { 
                    return rate < rhs.rate; 
                }
            }; // struct RefreshRate 

            Vector<RefreshRate, 4> refresh_rates;
        }; // struct SupportedResolution 
        ErrorOr<Vector<SupportedResolution>> supported_resolutions() const;

        Parser() = default;
        Parser(Parser&&) = default;
        Parser(Parser const&);
        Parser& operator=(Parser&&);
        Parser& operator=(Parser const&);

        bool operator==(Parser const& other) const;

        StringView version() const;

        auto bytes() const { return m_bytes; }

    private:
        Parser(ReadonlyBytes);
        Parser(ByteBuffer&&);

        ErrorOr<void> parse();

        template<typename T>
        T read_host(T const*) const;

        template<typename T>
        requires(IsIntegral<T> && sizeof(T) > 1) T read_le(T const*)
            const;

        template<typename T>
        requires(IsIntegral<T> && sizeof(T) > 1) T read_be(T const*)
            const;

        Definitions::EDID const& raw_edid() const;
        ErrorOr<IterationDecision> for_each_display_descriptor(Function<IterationDecision(u8, Definitions::DisplayDescriptor const&)>) const;

        ByteBuffer m_bytes_buffer;
        ReadonlyBytes m_bytes;
        u8 m_revision { 0 };
    #ifdef KERNEL
        OwnPtr<Kernel::KString> m_version;
    #else
        String m_version;
    #endif
        char m_legacy_manufacturer_id[4] {};
    }; // class Parser final 
} // namespace EDID
