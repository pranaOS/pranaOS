#pragma once

#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class TimeStampDumper {
public:
    explicit TimeStampDumper(const std::filesystem::path& path)
    {
        m_stream.open(path, std::ofstream::out);
    }

    ~TimeStampDumper()
    {
        if (m_stream.is_open()) {
            m_stream.close();
        }
    }

    void append(const std::string& path, int timestamp)
    {
        m_stream << path << " " << timestamp << "\n";
    }

private:
    std::ofstream m_stream {};
};