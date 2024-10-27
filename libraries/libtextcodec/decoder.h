/**
 * @file decoder.h
 * @author Krisna Pranav
 * @brief Decoder
 * @version 6.0
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>
#include <mods/function.h>

namespace TextCodec 
{

    class Decoder 
    {
    public:
        /**
         * @param on_code_point 
         */
        virtual void process(StringView, Function<void(u32)> on_code_point) = 0;

        /**
         * @return String 
         */
        virtual String to_utf8(StringView);

    protected:
        /**
         * @brief Destroy the Decoder object
         * 
         */
        virtual ~Decoder() = default;
    }; // class Decoder 

    class UTF8Decoder final : public Decoder 
    {
    public:
        /**
         * @param on_code_point 
         */
        virtual void process(StringView, Function<void(u32)> on_code_point) override;

        /**
         * @return String 
         */
        virtual String to_utf8(StringView) override;
    }; // class UTF8Decoder final : public Decoder 

    class UTF16BEDecoder final : public Decoder 
    {
    public:
        /**
         * @param on_code_point 
         */
        virtual void process(StringView, Function<void(u32)> on_code_point) override;

        /**
         * @return String 
         */
        virtual String to_utf8(StringView) override;
    }; // class UTF16BEDecoder final : public Decoder 

    class Latin1Decoder final : public Decoder 
    {
    public:
        /**
         * @param on_code_point 
         */
        virtual void process(StringView, Function<void(u32)> on_code_point) override;
    }; // class Latin1Decoder final : public Decoder 

    class Latin2Decoder final : public Decoder 
    {
    public:
        /**
         * @param on_code_point 
         */
        virtual void process(StringView, Function<void(u32)> on_code_point) override;
    }; // class Latin2Decoder final : public Decoder 

    class HebrewDecoder final : public Decoder 
    {
    public:
        /**
         * @param on_code_point 
         */
        virtual void process(StringView, Function<void(u32)> on_code_point) override;
    }; // class HebrewDecoder final : public Decoder 

    class CyrillicDecoder final : public Decoder 
    {
    public:
        /**
         * @param on_code_point 
         */
        virtual void process(StringView, Function<void(u32)> on_code_point) override;
    }; // class CyrillicDecoder final : public Decoder 

    class Koi8RDecoder final : public Decoder 
    {
    public:
        /**
         * @param on_code_point 
         */
        virtual void process(StringView, Function<void(u32)> on_code_point) override;
    }; // class Koi8RDecoder final : public Decoder 

    class Latin9Decoder final : public Decoder 
    {
    public:
        /**
         * @param on_code_point 
         */
        virtual void process(StringView, Function<void(u32)> on_code_point) override;
    }; // class Latin9Decoder final : public Decoder 

    class TurkishDecoder final : public Decoder 
    {
    public:
        /**
         * @param on_code_point 
         */
        virtual void process(StringView, Function<void(u32)> on_code_point) override;
    }; // class TurkishDecoder final : public Decoder 

    class XUserDefinedDecoder final : public Decoder 
    {
    public:
        /**
         * @param on_code_point 
         */
        virtual void process(StringView, Function<void(u32)> on_code_point) override;
    }; // class XUserDefinedDecoder final : public Decoder 

    /**
     * @param encoding 
     * @return Decoder* 
     */
    Decoder* decoder_for(String const& encoding);

    /**
     * @brief Get the standardized encoding object
     * 
     * @param encoding 
     * @return Optional<String> 
     */
    Optional<String> get_standardized_encoding(const String& encoding);

    /**
     * @return Decoder* 
     */
    Decoder* bom_sniff_to_decoder(StringView);
    
    /**
     * @return String 
     */
    String convert_input_to_utf8_using_given_decoder_unless_there_is_a_byte_order_mark(Decoder&, StringView);
} // namespace TextCodec