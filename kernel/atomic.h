/**
 * @file atomic.h
 * @author Krisna Pranav
 * @brief Atomic 
 * @version 0.1
 * @date 2023-05-29
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

enum class MemoryOrder {
	Relaxed = __ATOMIC_RELAXED,
	Acquire = __ATOMIC_ACQUIRE,
	Release = __ATOMIC_RELEASE,
	AcqRel = __ATOMIC_ACQ_REL,
	SeqCst = __ATOMIC_SEQ_CST,
	Consume = __ATOMIC_CONSUME
};

template<typename T, MemoryOrder default_order = MemoryOrder::SeqCst>
class Atomic {
public:
	Atomic() = default;
	Atomic(T value): m_val(value) {}

	inline T load(MemoryOrder order = default_order) const volatile noexcept {
		return __atomic_load_n(&m_val, (int) order);
	}

	inline void store(T val, MemoryOrder order = default_order) volatile noexcept {
		__atomic_store_n(&m_val, val, (int) order);
	}

	inline T add(T val, MemoryOrder order = default_order) volatile noexcept {
		return __atomic_fetch_add(&m_val, val, (int) order);
	}

	inline T sub(T val, MemoryOrder order = default_order) volatile noexcept {
		return __atomic_fetch_sub(&m_val, val, (int) order);
	}

	inline bool compare_exchange_strong(T& expected, T desired, MemoryOrder order = default_order) volatile noexcept {
		if (order == MemoryOrder::AcqRel || order == MemoryOrder::Release)
			return __atomic_compare_exchange(&m_val, &expected, &desired, false, (int) MemoryOrder::Release, (int) MemoryOrder::Acquire);
		return __atomic_compare_exchange(&m_val, &expected, &desired, false, (int) order, (int) order);
	}

private:
	T m_val;
};

