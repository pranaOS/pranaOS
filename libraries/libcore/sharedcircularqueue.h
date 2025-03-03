/**
 * @file sharedcircularqueue.h
 * @author Krisna Pranav
 * @brief shared circular queue
 * @version 6.0
 * @date 2025-03-03
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/assertions.h>
#include <mods/atomic.h>
#include <mods/builtinwrappers.h>
#include <mods/debug.h>
#include <mods/error.h>
#include <mods/format.h>
#include <mods/function.h>
#include <mods/nonnullrefptr.h>
#include <mods/numericlimits.h>
#include <mods/platform.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/string.h>
#include <mods/types.h>
#include <mods/variant.h>
#include <mods/weakable.h>
#include <libcore/anonymousbuffer.h>
#include <libcore/system.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/mman.h>

namespace Core 
{

    /**
     * @brief Construct a new requires object
     * 
     * @tparam T 
     * @tparam Size 
     */
    template<typename T, size_t Size = 32>
    requires(popcount(Size) == 1)
    class SharedSingleProducerCircularQueue final 
    {

    public:
        using ValueType = T;

        enum class QueueStatus : u8 {
            Invalid = 0,
            Full,
            Empty,
        }; // enum class QueueStatus : u8

        /**
         * @brief Construct a new SharedSingleProducerCircularQueue object
         * 
         */
        SharedSingleProducerCircularQueue() = default;

        /**
         * @brief Construct a new SharedSingleProducerCircularQueue object
         * 
         * @param queue 
         */
        SharedSingleProducerCircularQueue(SharedSingleProducerCircularQueue<ValueType, Size>& queue) = default;

        /**
         * @brief Construct a new Shared Single ProducerCircularQueue object
         * 
         * @param queue 
         */
        SharedSingleProducerCircularQueue(SharedSingleProducerCircularQueue&& queue) = default;

        /**
         * @param queue 
         * @return SharedSingleProducerCircularQueue& 
         */
        SharedSingleProducerCircularQueue& operator=(SharedSingleProducerCircularQueue&& queue) = default;

        /**
         * @return ErrorOr<SharedSingleProducerCircularQueue<T, Size>> 
         */
        static ErrorOr<SharedSingleProducerCircularQueue<T, Size>> try_create()
        {
            auto fd = TRY(System::anon_create(sizeof(SharedMemorySPCQ), O_CLOEXEC));
            return try_create_internal(fd, true);
        }

        /**
         * @param fd 
         * @return ErrorOr<SharedSingleProducerCircularQueue<T, Size>> 
         */
        static ErrorOr<SharedSingleProducerCircularQueue<T, Size>> try_create(int fd)
        {
            return try_create_internal(fd, false);
        }

        /**
         * @return constexpr size_t 
         */
        constexpr size_t size() const 
        { 
            return Size; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE size_t weak_remaining_capacity() const 
        { 
            return Size - weak_used(); 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE size_t weak_used() const
        {
            auto volatile head = m_queue->m_queue->m_tail.load(Mods::MemoryOrder::memory_order_relaxed);
            auto volatile tail = m_queue->m_queue->m_head.load(Mods::MemoryOrder::memory_order_relaxed);
            return head - tail;
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr int fd() const 
        { 
            return m_queue->m_fd; 
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr bool is_valid() const 
        { 
            return !m_queue.is_null(); 
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr size_t weak_head() const 
        { 
            return m_queue->m_queue->m_head.load(Mods::MemoryOrder::memory_order_relaxed); 
        }

        /**
         * @return ALWAYS_INLINE constexpr 
         */
        ALWAYS_INLINE constexpr size_t weak_tail() const 
        { 
            return m_queue->m_queue->m_tail.load(Mods::MemoryOrder::memory_order_relaxed); 
        }

        /**
         * @param to_insert 
         * @return ErrorOr<void, QueueStatus> 
         */
        ErrorOr<void, QueueStatus> try_enqueue(ValueType to_insert)
        {
            VERIFY(!m_queue.is_null());
            
            if (!can_enqueue())
                return QueueStatus::Full;

            auto our_tail = m_queue->m_queue->m_tail.load() % Size;

            m_queue->m_queue->m_data[our_tail] = to_insert;
            ++m_queue->m_queue->m_tail;

            return {};
        }   

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool can_enqueue() const
        {
            return ((head() - 1) % Size) != (m_queue->m_queue->m_tail.load() % Size);
        }

        /**
         * @param to_insert 
         * @param wait_function 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> try_blocking_enqueue(ValueType to_insert, Function<void()> wait_function)
        {
            ErrorOr<void, QueueStatus> result;

            while (true) {
                result = try_enqueue(to_insert);
                if (!result.is_error())
                    break;
                if (result.error() != QueueStatus::Full)
                    return Error::from_string_literal("Unexpected error while enqueuing"sv);

                wait_function();
            }

            return {};
        }

        /**
         * @return ErrorOr<ValueType, QueueStatus> 
         */
        ErrorOr<ValueType, QueueStatus> try_dequeue()
        {
            VERIFY(!m_queue.is_null());
            while (true) {
                if (head() >= m_queue->m_queue->m_tail.load())
                    return QueueStatus::Empty;

                auto size_max = NumericLimits<size_t>::max();

                if (m_queue->m_queue->m_head_protector.compare_exchange_strong(size_max, m_queue->m_queue->m_head.load())) {
                    auto old_head = m_queue->m_queue->m_head.load();
                    auto data = move(m_queue->m_queue->m_data[old_head % Size]);
                    m_queue->m_queue->m_head.fetch_add(1);
                    m_queue->m_queue->m_head_protector.store(NumericLimits<size_t>::max(), Mods::MemoryOrder::memory_order_release);
                    return { move(data) };
                }
            }
        }

        /**
         * @return size_t 
         */
        size_t head() const
        {
            return min(m_queue->m_queue->m_head.load(), m_queue->m_queue->m_head_protector.load());
        }

    private:
        struct SharedMemorySPCQ {
            SharedMemorySPCQ() = default;
            SharedMemorySPCQ(SharedMemorySPCQ const&) = delete;
            SharedMemorySPCQ(SharedMemorySPCQ&&) = delete;
            ~SharedMemorySPCQ() = default;

            CACHE_ALIGNED Atomic<size_t, Mods::MemoryOrder::memory_order_seq_cst> m_tail { 0 };
            CACHE_ALIGNED Atomic<size_t, Mods::MemoryOrder::memory_order_seq_cst> m_head { 0 };
            CACHE_ALIGNED Atomic<size_t, Mods::MemoryOrder::memory_order_seq_cst> m_head_protector { NumericLimits<size_t>::max() };

            alignas(ValueType) Array<ValueType, Size> m_data;
        }; // struct SharedMemorySPCQ

        class RefCountedSharedMemorySPCQ : public RefCounted<RefCountedSharedMemorySPCQ> 
        {
            friend class SharedSingleProducerCircularQueue;

        public:
            SharedMemorySPCQ* m_queue;
            void* m_raw;
            int m_fd;

            /**
             * @brief Destroy the Ref CountedSharedMemorySPCQ object
             * 
             */
            ~RefCountedSharedMemorySPCQ()
            {
                MUST(System::close(m_fd));
                MUST(System::munmap(m_raw, sizeof(SharedMemorySPCQ)));
                dbgln_if(SHARED_QUEUE_DEBUG, "destructed SSPCQ at {:p}, shared mem: {:p}", this, this->m_raw);
            }

        private:
            /**
             * @brief Construct a new RefCountedSharedMemorySPCQ object
             * 
             * @param queue 
             * @param fd 
             */
            RefCountedSharedMemorySPCQ(SharedMemorySPCQ* queue, int fd)
                : m_queue(queue)
                , m_raw(reinterpret_cast<void*>(queue))
                , m_fd(fd)
            {
            }
        }; // class RefCountedSharedMemorySPCQ : public RefCounted<RefCountedSharedMemorySPCQ> 

        /**
         * @param fd 
         * @param is_new 
         * @return ErrorOr<SharedSingleProducerCircularQueue<T, Size>> 
         */
        static ErrorOr<SharedSingleProducerCircularQueue<T, Size>> try_create_internal(int fd, bool is_new)
        {
            auto name = String::formatted("SharedSingleProducerCircularQueue@{:x}", fd);
            auto* raw_mapping = TRY(System::mmap(nullptr, sizeof(SharedMemorySPCQ), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0, 0, name));

            dbgln_if(SHARED_QUEUE_DEBUG, "successfully mmapped {} at {:p}", name, raw_mapping);

            SharedMemorySPCQ* shared_queue = is_new ? new (raw_mapping) SharedMemorySPCQ() : reinterpret_cast<SharedMemorySPCQ*>(raw_mapping);

            if (!shared_queue)
                return Error::from_string_literal("Unexpected error when creating shared queue from raw memory"sv);

            return SharedSingleProducerCircularQueue<T, Size> { move(name), adopt_ref(*new (nothrow) RefCountedSharedMemorySPCQ(shared_queue, fd)) };
        }

        /**
         * @brief Construct a new Shared SingleProducerCircularQueue object
         * 
         * @param name 
         * @param queue 
         */
        SharedSingleProducerCircularQueue(String name, RefPtr<RefCountedSharedMemorySPCQ> queue)
            : m_queue(queue)
            , m_name(move(name))
        {
        }

        RefPtr<RefCountedSharedMemorySPCQ> m_queue;

        String m_name {};
    }; // class SharedSingleProducerCircularQueue final

} // namespace Core
