/**
 * @file noncopyable.h
 * @author Krisna Pranav
 * @brief NonCopyable
 * @version 6.0
 * @date 2023-06-29
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#define MOD_MAKE_NONCOPYABLE(c) \
  private:                     \
    c(c const&) = delete;      \
    c& operator=(c const&) = delete

#define MOD_MAKE_NONMOVABLE(c) \
  private:                    \
    c(c&&) = delete;          \
    c& operator=(c&&) = delete