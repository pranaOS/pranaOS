/**
 * @file field.cpp
 * @author Krisna Pranav
 * @brief Field
 * @version 6.0
 * @date 2023-06-17
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#include "field.hpp"
#include <functional>
#include <log/log.hpp>

/**
 * @brief getCString
 *
 * @return const char*
 */
const char* Field::getCString() const
{
    return mValue.c_str();
}

/**
 * @brief getString
 *
 * @return const std::string&
 */
const std::string& Field::getString() const
{
    return mValue;
}

/**
 * @brief lget
 *
 * @tparam T
 * @param val
 * @param mValue
 * @param foo
 * @return T
 */
template <class T>
static T lget(T& val, const std::string& mValue, std::function<T()> foo)
{
    val = 0;
    if(mValue.empty())
    {
        return val;
    }

    try
    {
        return foo();
    }
    catch(...)
    {
        LOG_FATAL("Can't convert value to: %s", typeid(T).name());
    }
    return val;
}

/**
 * @brief getFloat
 *
 * @return float
 */
float Field::getFloat() const
{
    float ret;
    return lget<float>(ret, mValue, [=]() -> float
                       { return std::stof(mValue); });
}

/**
 * @brief getBool
 *
 * @return true
 * @return false
 */
bool Field::getBool() const
{
    bool ret;
    return lget<bool>(ret, mValue, [=]() -> bool
                      { return std::stoi(mValue) > 0; });
}

/**
 * @brief getDouble
 *
 * @return double
 */
double Field::getDouble() const
{
    double ret;
    return lget<double>(ret, mValue, [=]() -> double
                        { return std::stod(mValue); });
}

/**
 * @brief getInt8
 *
 * @return std::int8_t
 */
std::int8_t Field::getInt8() const
{
    return !mValue.empty() ? static_cast<std::int8_t>(atol(mValue.c_str())) : static_cast<std::int8_t>(0);
}

/**
 * @brief getInt32
 *
 * @return std::int32_t
 */
std::int32_t Field::getInt32() const
{
    return !mValue.empty() ? static_cast<std::int32_t>(atol(mValue.c_str())) : static_cast<std::int32_t>(0);
}

/**
 * @brief getUInt8
 *
 * @return std::uint8_t
 */
std::uint8_t Field::getUInt8() const
{
    return !mValue.empty() ? static_cast<std::uint8_t>(atol(mValue.c_str())) : static_cast<std::uint8_t>(0);
}

/**
 * @brief getUInt16
 *
 * @return std::uint16_t
 */
std::uint16_t Field::getUInt16() const
{
    return !mValue.empty() ? static_cast<std::uint16_t>(atol(mValue.c_str())) : static_cast<std::uint16_t>(0);
}

/**
 * @brief getInt16
 *
 * @return std::int16_t
 */
std::int16_t Field::getInt16() const
{
    return !mValue.empty() ? static_cast<std::int16_t>(atol(mValue.c_str())) : static_cast<std::int16_t>(0);
}

/**
 * @brief getUInt32
 *
 * @return std::uint32_t
 */
std::uint32_t Field::getUInt32() const
{
    std::uint32_t ret;
    return lget<std::uint32_t>(ret, mValue, [=]() -> unsigned long
                               { return std::stoul(mValue); });
}

/**
 * @brief getUInt64
 *
 * @return std::uint64_t
 */
std::uint64_t Field::getUInt64() const
{
    std::uint64_t ret;
    return lget<std::uint64_t>(ret, mValue, [=]() -> unsigned long long
                               { return std::stoull(mValue); });
}

/**
 * @brief getInt64
 *
 * @return std::int64_t
 */
std::int64_t Field::getInt64() const
{
    std::int64_t ret;
    return lget<std::int64_t>(ret, mValue, [=]() -> long long
                              { return std::stoll(mValue); });
}

/**
 * @brief setValue
 *
 * @param value
 */
void Field::setValue(const char* value)
{
    mValue = value;
}