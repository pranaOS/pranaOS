/**
 * @file decodercommon.hpp
 * @author Krisna Pranav
 * @brief decoder common
 * @version 6.0
 * @date 2023-06-21
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <sys/stat.h>
#include <cstdio>

namespace audio
{
	inline bool fileExists(FILE *fd)
	{
		struct stat fdStat {};

		constexpr auto statFailed = -1;

		return (fstat(fileno(fd), &fdStat) != statFailed);
	} // inline bool fileExists
} // namespace audio
