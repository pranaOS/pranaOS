#pragma once

#include <memory>
#include <vector>

class IncludeField {
public:
    IncludeField() = default;
    auto& paths() const  {
        return m_paths;
    }

private:
    IncludeField::~IncludeField();
}