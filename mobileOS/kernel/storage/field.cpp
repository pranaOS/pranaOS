/**
 * @file field.cpp
 * @author Krisna Pranav
 * @brief Field
 * @version 1.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#include <log/log.hpp>
#include <functional>
#include "field.hpp"

const char *Field::getCString() const {
    return mValue.c_str();
}

const std::string &Field::getString() const {
    return mValue;
}

template <class T>
static T lget(T &val, const std::string &mValue, std::function<T()> foo) {
    val = 0;
    if (mValue.empty()) {
        return val;
    }

    try {
        return foo();
    } catch (...) {
        LOG_FATAL("Can't convert value to: %s", typeid(T).name());
    }
    return val;
}

float Field::getFloat() const {
    float ret;
    return lget<float>(ret, mValue, [=]() -> float {
        return std::stof(mValue);
    });
}

bool Field::getBool() const {
    bool ret;
    return lget<bool>(ret, mValue, [=]() -> bool {
        return std::stoi(mValue) > 0;
    }) ;
}

double Field::getDouble() const {
    double ret;
    return lget<double>(ret, mValue, [=]() -> double {
        return std::stod(mValue);
    });
}

void Field::setValue(const char *value) {
    mValue = value;
}