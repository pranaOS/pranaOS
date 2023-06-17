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
     * @brief runtime_error
     * 
     */
    using std::runtime_error::runtime_error;   
};

class Database {
public:
    /**
     * @brief Construct a new Database objectd
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

    /**
     * @brief query
     * 
     * @param format 
     * @param ... 
     * @return std::unique_ptr<QueryResult> 
     */
    std::unique_ptr<QueryResult> query(const char *format, ...);

    /**
     * @brief execute
     * 
     * @param format 
     * @param ... 
     * @return true 
     * @return false 
     */
    bool execute(const char *format, ...);

    static bool initalize();

    static bool deinitialize();

    /**
     * @brief storeIntoFile
     * 
     * @param syncPath 
     * @return true 
     * @return false 
     */
    bool storeIntoFile(const std::filesystem::path &syncPath);

protected:
    sqlite3 *dbConnection;
    std::string dbName;
    char *queryStatementResult;
    bool isInitialized_;
};