/**
 * @file byte_buffer.h
 * @author Krisna Pranav
 * @brief Byte Buffer
 * @version 1.0 
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "nonnull_refptr.h"
#include "refcounted.h"
#include "span.h"
#include "refptr.h"
#include "stdlibextra.h"
#include "kmalloc.h"

namespace Mods {
    class ByteBufferImpl : public RefCounted<ByteBufferImpl> {
    public:
        /**
         * @param size 
         * @return NonnullRefPtr<ByteBufferImpl> 
         */
        static NonnullRefPtr<ByteBufferImpl> create_uninitialized(size_t size);

        /**
         * @return NonnullRefPtr<ByteBufferImpl> 
         */
        static NonnullRefPtr<ByteBufferImpl> create_zeroed(size_t);

        /**
         * @return NonnullRefPtr<ByteBufferImpl> 
         */
        static NonnullRefPtr<ByteBufferImpl> copy(const void*, size_t);

        /**
         * @return NonnullRefPtr<ByteBufferImpl> 
         */
        static NonnullRefPtr<ByteBufferImpl> wrap(void*, size_t);

        /**
         * @return NonnullRefPtr<ByteBufferImpl> 
         */
        static NonnullRefPtr<ByteBufferImpl> adopt(void*, size_t);
    };
}