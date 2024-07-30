/**
 * @file logging.hpp
 * @author Krisna Pranav
 * @brief Logging
 * @version 6.0
 * @date 2023-06-17
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#if DEBUG_DB_MODEL_DATA == 1
#define debug_db_data(...) LOG_DEBUG(__VA_ARGS__)
#define error_db_data(...) LOG_ERROR(__VA_ARGS__)
#else
#define debug_db_data(...)
#define error_db_data(...)
#endif