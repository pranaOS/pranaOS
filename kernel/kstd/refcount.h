/**
 * @file refcount.h
 * @author Krisna Pranav
 * @brief RefCount
 * @version 6.0
 * @date 2023-06-19
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "../atomic.h"
#include "kstdio.h"
#include "utility.h"

namespace kstd
{
enum class PtrReleaseAction
{
    Keep,
    Destroy
};

class RefCount
{
  public:
    /**
     * @brief Construct a new Ref Count object
     *
     * @param strong_count
     */
    explicit RefCount(int strong_count);

    /**
     * @brief Construct a new Ref Count object
     *
     * @param other
     */
    RefCount(RefCount&& other);

    /**
     * @brief Construct a new Ref Count object
     *
     * @param other
     */
    RefCount(const RefCount& other) = delete;

    /**
     * @brief Destroy the Ref Count object
     *
     */
    ~RefCount();

    /**
     * @brief strong_count
     *
     * @return long
     */
    long strong_count() const;

    /**
     * @brief weak_count
     *
     * @return long
     */
    long weak_count() const;

    /**
     * @brief make_strong
     *
     * @return true
     * @return false
     */
    bool make_strong();

    /**
     * @brief acqure_strong
     *
     */
    void acquire_strong();

    /**
     * @brief acquire_weak
     *
     */
    void acquire_weak();

    /**
     * @brief release_strong
     *
     * @return PtrReleaseAction
     */
    PtrReleaseAction release_strong();

    /**
     * @brief release_weak
     *
     */
    void release_weak();

  private:
    /**
     * @brief atomic m_strong_cout
     *
     */
    Atomic<long, MemoryOrder::SeqCst> m_strong_count = 0;

    /**
     * @brief m_weak_cout
     *
     */
    Atomic<long, MemoryOrder::SeqCst> m_weak_count = 0;
};
} // namespace kstd