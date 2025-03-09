/**
 * @file name.h
 * @author Krisna Pranav
 * @brief name
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>
#include <mods/string.h>

namespace DNS 
{

    class Name 
    {
    public:
        /**
         * @brief Construct a new Name object
         * 
         */
        Name() = default;

        /**
         * @brief Construct a new Name object
         * 
         */
        Name(String const&);

        /**
         * @param data 
         * @param offset 
         * @param max_offset 
         * @param recursion_level 
         * @return Name 
         */
        static Name parse(u8 const* data, size_t& offset, size_t max_offset, size_t recursion_level = 0);

        /**
         * @return size_t 
         */
        size_t serialized_size() const;

        /**
         * @return String const& 
         */
        String const& as_string() const 
        { 
            return m_name; 
        }

        void randomize_case();

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Name const& other) const 
        { 
            return Traits::equals(*this, other); 
        }

        class Traits : public Mods::Traits<Name> 
        {
        public:
            /**
             * @param name 
             * @return unsigned 
             */
            static unsigned hash(Name const& name);

            /**
             * @return true 
             * @return false 
             */
            static bool equals(Name const&, Name const&);
        }; // class Traits : public Mods::Traits<Name> 

    private:
        String m_name;
    }; // class Name

    /**
     * @param stream 
     * @return OutputStream& 
     */
    OutputStream& operator<<(OutputStream& stream, Name const&);

} // namespace DNS

/**
 * @tparam  
 */
template<>
struct Mods::Formatter<DNS::Name> : Formatter<StringView> {
    /**
     * @param builder 
     * @param value 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> format(FormatBuilder& builder, DNS::Name const& value)
    {
        return Formatter<StringView>::format(builder, value.as_string());
    }
};
