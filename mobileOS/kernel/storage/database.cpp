/**
 * @file database.cpp
 * @author Krisna Pranav
 * @brief Database
 * @version 1.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#include <log/log.hpp>
#include <gsl/util>
#include <cstring>
#include "database.hpp"

extern sqlite3_vfs *sqlite3_ecophonevfs(void);

[[nodiscard]] static bool isNotPragmaRelated(const char *msg) {
    return nullptr == strstr(msg, "PRAGMA");
}

extern "C" {
    int sqlite3_os_init(void) {
        sqlite3_vfs_register(sqlite3_ecophonevfs(), 1);
        return SQLITE_OK;
    }

    int sqlite3_os_end(void) {
        return SQLITE_OK;
    }

    void errorCallBack(void *pArg, int iErrCode, const char *zMsg)  {
        if (isNotPragmaRelated(zMsg)) {
            LOG_ERROR("%d %s\n", iErrCode, zMsg);
        }
    }
}