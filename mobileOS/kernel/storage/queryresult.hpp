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
    QueryResult();
    
    virtual ~QueryResult(){};

    const Field& operator[](int index) const {
        return rows[currentRow][index];
    };

    bool nextRow();

    uint32_t getFieldCount() const {
        return rows[currentRow].size();
    }

    uint32_t getRowCount() const {
        return rows.size();
    }

private:
    std::vector<std::vector<Field>> rows;
    uint32_t currentRow;
};