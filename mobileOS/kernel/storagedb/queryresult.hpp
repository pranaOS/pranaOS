/**
 * @file queryresult.hpp
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include "field.hpp"

class QueryResult {
public:
    /**
     * @brief Construct a new Query Result objec
     * 
     */
    QueryResult();

    /**
     * @brief Destroy the Query Result object
     * 
     */
    virtual ~QueryResult(){};

    /**
     * @brief field 
     * 
     * @param index 
     * @return const Field& 
     */
    const Field &operator[](int index) const {
        return rows[currentRow][index];
    }

    /**
     * @brief nextRow
     * 
     * @return true 
     * @return false 
     */
    bool nextRow();

    /**
     * @brief addRow
     * 
     * @param row 
     */
    void addRow(const std::vector<Field> &row);

    /**
     * @brief Get the Field Count object
     * 
     * @return uint32_t 
     */
    uint32_t getFieldCount() const {
        return rows[currentRow].size();
    }

    /**
     * @brief Get the Row Count object
     * 
     * @return uint32_t 
     */
    uint32_t getRowCount() const {
        return rows.size();
    }

private:
    std::vector<std::vector<Field>> rows;
    uint32_t currentRow;
};