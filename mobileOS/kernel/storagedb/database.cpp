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

#include "database.hpp"
#include <log/log.hpp>
#include <gsl/util>
#include <cstring>

extern sqlite3_vfs *sqlite3_ecophonevfs(void);

/**
 * @brief isNotPragmaRelated
 * 
 * @param msg 
 * @return true 
 * @return false 
 */
[[nodiscard]] static bool isNotPragmaRelated(const char *msg) {
    return nullptr == strstr(msg, "PRAGMA");
}

extern "C" {
    /**
     * @brief sqlite3_os_init
     * 
     * @return int 
     */
    int sqlite3_os_init(void) {
        sqlite3_vfs_register(sqlite3_ecophonevfs(), 1);
        return SQLITE_OK;
    }

    /**
     * @brief sqlite3_os_end
     * 
     * @return int 
     */
    int sqlite3_os_end(void) {
        return SQLITE_OK;
    }

    /**
     * @brief errorCallback
     * 
     * @param pArg 
     * @param iErrCode 
     * @param zMsg 
     */
    void errorLogCallback(void *pArg, int iErrCode, const char *zMsg) {
        if (isNotPragmaRelated(zMsg)) {
            LOG_ERROR("(%d) %s\n", iErrCode, zMsg);
        }
    }
}

/**
 * @brief dbApplicationID, enabled
 * 
 */
constexpr auto dbApplicationId = 0x65727550; 
constexpr auto enabled         = 1;

/**
 * @brief Construct a new Database:: Database object
 * 
 * @param name 
 * @param readOnly 
 */
Database::Database(const char *name, bool readOnly)
    : dbConnection(nullptr), dbName(name), queryStatementBuffer{nullptr}, isInitialized_(false) {
    const int flags = (readOnly) ? (SQLITE_OPEN_READONLY) : (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    LOG_INFO("Opening database: %s", dbName.c_str());

    if (const auto rc = sqlite3_open_v2(name, &dbConnection, flags, nullptr); rc != SQLITE_OK) {
        LOG_ERROR("SQLITE INITIALIZATION ERROR! rc=%d dbName=%s", rc, name);
        throw DatabaseInitialisationError{"Failed to initialize the sqlite db"};
    }
    sqlite3_extended_result_codes(dbConnection, enabled);
    initQueryStatementBuffer();
    pragmaQuery("PRAGMA integrity_check;");
    pragmaQuery("PRAGMA locking_mode=EXCLUSIVE");

    if (isInitialized_ = pragmaQueryForValue("PRAGMA application_id;", dbApplicationId); not isInitialized_) {
        populateDbAppId();
        isInitialized_ = true;
    }
    LOG_DEBUG("Database %s initialized", dbName.c_str());
}

/**
 * @brief populateDbAppId
 * 
 */
void Database::populateDbAppId() {
    std::stringstream setAppIdPragma;
    setAppIdPragma << "PRAGMA application_id=" << dbApplicationId << ";";
    pragmaQuery(setAppIdPragma.str());
}

/**
 * @brief initQueryStatementBuffer
 * 
 */
void Database::initQueryStatementBuffer() {
    queryStatementBuffer = static_cast<char *>(sqlite3_malloc(maxQueryLen));
    if (queryStatementBuffer == nullptr) {
        LOG_ERROR("Unable to allocate memory for query statement buffer.");
        throw DatabaseInitialisationError{"Failed to initialize the query statement buffer"};
    }
    clearQueryStatementBuffer();
}

/**
 * @brief clearQueryStatementBuffer
 * 
 */
void Database::clearQueryStatementBuffer() {
    std::memset(queryStatementBuffer, 0, maxQueryLen);
}

/**
 * @brief Destroy the Database:: Database object
 * 
 */
Database::~Database() {
    sqlite3_free(queryStatementBuffer);
    sqlite3_close(dbConnection);
}

/**
 * @brief initialize
 * 
 * @return true 
 * @return false 
 */
bool Database::initialize() {
    if (const auto code = sqlite3_config(SQLITE_CONFIG_LOG, errorLogCallback, (void *)1); code != SQLITE_OK) {
        return false;
    }
    return sqlite3_initialize() == SQLITE_OK;
}

/**
 * @brief deinitialize
 * 
 * @return true 
 * @return false 
 */
bool Database::deinitialize() {
    return sqlite3_shutdown() == SQLITE_OK;
}

/**
 * @brief execute
 * 
 * @param format 
 * @param ... 
 * @return true 
 * @return false 
 */
bool Database::execute(const char *format, ...) {
    if (format == nullptr) {
        return false;
    }

    auto cleanup = gsl::finally([this] { clearQueryStatementBuffer(); });

    va_list ap;
    va_start(ap, format);
    sqlite3_vsnprintf(maxQueryLen, queryStatementBuffer, format, ap);
    va_end(ap);
    if (const int result = sqlite3_exec(dbConnection, queryStatementBuffer, nullptr, nullptr, nullptr);
        result != SQLITE_OK) {
        LOG_ERROR("Execution of query failed with %d, errcode: %d, extended errcode: %d",
                  result,
                  sqlite3_errcode(dbConnection),
                  sqlite3_extended_errcode(dbConnection));
        return false;
    }
    return true;
}

/**
 * @brief query
 * 
 * @param format 
 * @param ... 
 * @return std::unique_ptr<QueryResult> 
 */
std::unique_ptr<QueryResult> Database::query(const char *format, ...) {
    if (format == nullptr) {
        return nullptr;
    }

    auto cleanup = gsl::finally([this] { clearQueryStatementBuffer(); });

    va_list ap;
    va_start(ap, format);
    sqlite3_vsnprintf(maxQueryLen, queryStatementBuffer, format, ap);
    va_end(ap);

    auto queryResult = std::make_unique<QueryResult>();
    if (const int result = sqlite3_exec(dbConnection, queryStatementBuffer, queryCallback, queryResult.get(), nullptr);
        result != SQLITE_OK) {
        if (isNotPragmaRelated(queryStatementBuffer)) {
            LOG_ERROR("SQL query failed selecting : %d", result);
        }
        return nullptr;
    }
    return queryResult;
}

/**
 * @brief queryCallback
 * 
 * @param usrPtr 
 * @param count 
 * @param data 
 * @param columns 
 * @return int 
 */
int Database::queryCallback(void *usrPtr, int count, char **data, char **columns) {
    QueryResult *db = reinterpret_cast<QueryResult *>(usrPtr);

    std::vector<Field> row;
    for (uint32_t i = 0; i < (uint32_t)count; i++) {
        try {
            row.push_back(Field{data[i]});
        }
        catch (...) {
            LOG_FATAL("Error on column: %" PRIu32, i);
        }
    }

    db->addRow(row);

    return 0;
}

/**
 * @brief getLastInsertRowId
 * 
 * @return uint32_t 
 */
uint32_t Database::getLastInsertRowId() {
    return sqlite3_last_insert_rowid(dbConnection);
}

/**
 * @brief pragmaQueryForValue
 * 
 * @param pragmaStatement 
 * @param value 
 * @return true 
 * @return false 
 */
auto Database::pragmaQueryForValue(const std::string &pragmaStatement, const std::int32_t value) -> bool {
    auto results = query(pragmaStatement.c_str());

    if (!results || results->getRowCount() == 0) {
        LOG_DEBUG("no results!");
        return false;
    }

    do {
        const auto fieldsCount = results->getFieldCount();
        for (uint32_t i = 0; i < fieldsCount; i++) {
            Field field{(*results)[i]};
            if (field.getInt32() == value) {
                LOG_DEBUG("Found the match: %" PRIx32, value);
                return true;
            }
        }
    } while (results->nextRow());

    return false;
}

/**
 * @brief pragmaQuery
 * 
 * @param pragmaStatement 
 */
void Database::pragmaQuery(const std::string &pragmaStatement) {
    auto results = query(pragmaStatement.c_str());

    if (results && results->getRowCount()) {
        do {
            const auto fieldsCount = results->getFieldCount();
            for (uint32_t i = 0; i < fieldsCount; i++) {
                Field field = (*results)[i];
            }
        } while (results->nextRow());
    }
    else {
        LOG_DEBUG("no results!");
    }
}

/**
 * @brief storeIntoFile
 * 
 * @param syncPath 
 * @return true 
 * @return false 
 */
bool Database::storeIntoFile(const std::filesystem::path &syncPath) {
    if (sqlite3_get_autocommit(dbConnection) == 0) {
        if (const auto rc = execute("COMMIT;"); !rc) {
            LOG_ERROR("failed to execute commit; sqlite3 autocommit after commit: %d",
                      sqlite3_get_autocommit(dbConnection));
            return false;
        }
        LOG_INFO("sqlite3 autocommit after commit: %d", sqlite3_get_autocommit(dbConnection));
    }

    LOG_INFO("Store database: %s, into file: %s - STARTED", dbName.c_str(), syncPath.c_str());
    if (const auto rc = execute("VACUUM main INTO '%q';", syncPath.c_str()); !rc) {
        LOG_ERROR("Store database: %s, into file: %s - FAILED", dbName.c_str(), syncPath.c_str());
        return false;
    }
    LOG_INFO("Store database: %s, into file: %s - SUCCEEDED", dbName.c_str(), syncPath.c_str());
    return true;
}