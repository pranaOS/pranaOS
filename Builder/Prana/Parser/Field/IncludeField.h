#pragma once

#include <memory>
#include <string>
#include <vector>

class IncludeField {
public:

    IncludeField() = default;
    void add_path(const std::shared_ptr<std::string>& path) { m_paths.push_back(path); }
    auto& paths() const { return m_paths; }

private:
    std::vector<std::shared_ptr<std::string>> m_paths {};
};