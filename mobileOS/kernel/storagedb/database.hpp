/**
 * @file database.hpp
 * @author Krisna Pranav
 * @brief Database
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <memory>
#include <stdexcept>
#include <filesystem>
#include "sqlite3.h"
#include "queryresult.hpp"

class DatabaseInitialisationError : public std::runtime_error {
public:
    /**
     * @brief runtime
     * 
     */
    using std::runtime_error::runtime_error;
};

class Database {
public:
    
    /**
     * @brief Construct a new Database object
     * 
     * @param name 
     * @param readOnly 
     */
    explicit Database(const char *name, bool readOnly = false);

    /**
     * @brief Destroy the Database object
     * 
     */
    virtual ~Database();

    std::unique_ptr<QueryResult> query(const char *format, ...);

    /**
     * execute
    */
    bool execute(const char *format, ...);

    /**
     * @brief initialize
     * 
     * @return true 
     * @return false 
     */
    static bool initialize();

    /**
     * @brief deinitialize
     * 
     * @return true 
     * @return false 
     */
    static bool deinitialize();

    /**
     * @brief storeIntoFile
     * 
     * @param syncPath 
     * @return true 
     * @return false 
     */
    bool storeIntoFile(const std::filesystem::path &syncPath);

    /**
     * @brief Get the Last Insert Row Id object
     * 
     * @return uint32_t 
     */
    uint32_t getLastInsertRowId();

    /**
     * @brief pragmaQuery
     * 
     * @param pragmaStatement 
     */
    void pragmaQuery(const std::string &pragmaStatement);

    /**
     * @brief pragmaQueryForValue
     * 
     * @param pragmaStatement 
     * @param value 
     * @return true 
     * @return false 
     */
    auto pragmaQueryForValue(const std::string &pragmaStatement, const std::int32_t value) -> bool;

    /**
     * @brief isInitialized
     * 
     * @return true 
     * @return false 
     */
    [[nodiscard]] bool isInitialized() const noexcept {
        return isInitialized_;
    }

    /**
     * @brief Get the Name object
     * 
     * @return std::string 
     */
    [[nodiscard]] std::string getName() const {
        return dbName;
    }

private:
    /**
     * @brief maxQueryLen
     * 
     */
    static constexpr std::uint32_t maxQueryLen = (8 * 1024);

    void initQueryStatementBuffer();
    void clearQueryStatementBuffer();

    void populateDbAppId();

    /**
     * @brief queryCallback
     * 
     * @param usrPtr 
     * @param count 
     * @param data 
     * @param columns 
     * @return int 
     */
    static int queryCallback(void *usrPtr, int count, char **data, char **columns);

protected:
    sqlite3 *dbConnection;
    std::string dbName;
    char *queryStatementBuffer;
    bool isInitialized_;
};