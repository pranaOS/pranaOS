/**
 * @file platform.hpp
 * @author Krisna Pranav
 * @brief platform
 * @version 6.0
 * @date 2024-08-27
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

namespace platform
{
	class Platform
	{
	public:
		virtual ~Platform() = default;
		virtual void init() = 0;
		virtual void deinit() = 0;
	}; // class Platform
} // namespace platform
