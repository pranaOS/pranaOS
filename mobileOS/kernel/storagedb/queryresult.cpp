/**
 * @file queryresult.cpp
 * @author Krisna Pranav
 * @brief QueryResult
 * @version 6.0
 * @date 2023-06-17
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#include "queryresult.hpp"

/**
 * @brief Construct a new Query Result:: Query Result object
 *
 */
QueryResult::QueryResult()
    : currentRow(0)
{
}

/**
 * @brief addRow
 *
 * @param row
 */
void QueryResult::addRow(const std::vector<Field>& row)
{
    rows.push_back(row);
}

/**
 * @brief nextRow
 *
 * @return true
 * @return false
 */
bool QueryResult::nextRow()
{
    ++currentRow;

    return (currentRow < rows.size());
}