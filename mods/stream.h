/**
 * @file stream.h
 * @author Krisna Pranav
 * @brief Stream
 * @version 1.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "concept.h"
#include "endian.h"
#include "forward.h"
#include "optional.h"
#include "span.h"
#include "stdlibextra.h"

namespace Mods::Detail {
    class Stream {
    public:
        /**
         * @brief Destroy the Stream object
         * 
         */
        virtual ~Stream() {
            ASSERT(!has_any_error());
        }

        /**
         * @return true 
         * @return false 
         */
        virtual bool has_recoverable_error() const {
            return m_fatal_error;
        };

        virtual has_fatal_error() const {
            return m_fatal_error;
        };

        virtual bool has_any_error() const {
                return has_recoverable_error() || has_fatal_error();
        };

        virtual bool handle_fatal_error() {
            return exchange(m_fatal_error);
        };

        /** 
         * @return true 
         * @return false 
         */
        virtual bool handle_any_error() {
            if (has_any_error()) {
                m_recoverable_error = false;
                m_fatal_error = false;

                return true;
            }

            return false;
            
        }

        virtual bool set_recoverable_error() const {
            m_recoverable_error = true;
        }

    private:
        mutable bool m_recoverable_error { false };
        mutable bool m_fatal_error { false };
    };

    namespace Mods {
        class InputStream : public virtual Mods::Detail::Stream {
        public:
            /**
             * @return size_t 
             */
            virtual size_t read(Bytes) = 0;

            /**
             * @return true 
             * @return false 
             */
            virtual bool unreliable_eof() const = 0;

            /**
             * @return true 
             * @return false 
             */
            virtual bool read_or_error(Bytes) = 0;

            /** 
             * @param count 
             * @return true 
             * @return false 
             */
            virtual bool discord_or_error(size_t count) = 0;
        };
    }

}

