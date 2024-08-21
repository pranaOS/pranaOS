/**
 * @file optional.h
 * @author Krisna Pranav
 * @brief Optional
 * @version 6.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "kstdio.h"
#include "utility.h"

namespace kstd {
	struct nullopt_t {};

	extern nullopt_t nullopt;

	template<typename T>
	class Optional {
	public:
		Optional(const T& value):
			m_value(new T(value))
		{}

        /**
         * @brief Construct a new Optional object
         * 
         * @param value 
         */
		Optional(T&& value) noexcept:
			m_value(new T(value))
		{}

        /**
         * @brief Construct a new Optional object
         * 
         */
		Optional(nullopt_t):
			m_value(nullptr)
		{}

        /**
         * @brief Construct a new Optional object
         * 
         * @param other 
         */
		Optional(const Optional<T>& other):
			m_value(other.m_value ? new T(*other.m_value) : nullptr)
		{}
        
        /**
         * @brief Construct a new Optional object
         * 
         * @tparam U 
         * @param other 
         */
		template<typename U>
		Optional(const Optional<U>& other):
				m_value(other.m_value ? new T(*static_cast<T*>(other.m_value)) : nullptr)
		{}

        /**
         * @brief Construct a new Optional object
         * 
         * @param other 
         */
		Optional(Optional<T>&& other) noexcept:
				m_value(other.m_value)
		{
			other.m_value = nullptr;
		}

        /**
         * @brief Construct a new Optional object
         * 
         * @tparam U 
         * @param other 
         */
		template<typename U>
		Optional(Optional<U>&& other) noexcept:
				m_value(static_cast<T*>(other.m_value))
		{
			other.m_value = nullptr;
		}

        /**
         * @brief Destroy the Optional object
         * 
         */
		~Optional() {
			delete m_value;
		}

        /**
         * @brief hasvalue
         * 
         * @return true 
         * @return false 
         */
		[[nodiscard]] bool has_value() const {
			return m_value;
		}

        /**
         * @brief bool
         * 
         * @return true 
         * @return false 
         */
		[[nodiscard]] operator bool() const {
			return m_value;
		}

        /**
         * @brief value
         * 
         * @return const T& 
         */
		const T& value() const {
			ASSERT(m_value);
			return *m_value;
		}
        
        /**
         * @brief operator
         * 
         * @param other 
         * @return Optional<T>& 
         */
		Optional<T>& operator =(const Optional<T>& other) {
			if(&other == this)
				return *this;
			m_value = new T(other.m_value);
		}

        /**
         * @brief operator
         * 
         * @param other 
         * @return Optional<T>& 
         */
		Optional<T>& operator =(Optional<T>&& other) noexcept {
			kstd::swap(other.m_value, m_value);
		}

	private:
		T* m_value;
	};
}