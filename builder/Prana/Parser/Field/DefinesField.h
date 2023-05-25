#pragma once

#include <string>
#include <unordered_map>

class DefinesField {
public:
    DefinesField() = default;

    inline void add_defines(const std::string& key, const std::vector<std::shared_ptr<std::string>>& values)
    {
        for (auto& value : values) {
            add_define(key, value);
        }
    }

    inline void write_defines(const std::string& key, std::vector<std::shared_ptr<std::string>>&& values)
    {
        m_defines[key] = std::move(values);
    }

    auto& defines() { return m_defines; }

private:
    inline bool add_define(const std::string& key, const std::shared_ptr<std::string>& value)
    {
        m_defines[key].push_back(value);
        return true;
    }

private:
    std::unordered_map<std::string, std::vector<std::shared_ptr<std::string>>> m_defines;
};