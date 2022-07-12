#pragma once

#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class TimeStampParser {
public:
    explicit TimeStampParser(const std::filesystem::path& path)
    {
        m_stream.open(path, std::ifstream::in);
    }

    ~TimeStampParser()
    {
        if (m_stream.is_open()) {
            m_stream.close();
        }
    }

    void run(const std::function<void(const std::string& path, int timestamp)>& callback)
    {
        std::string cur_line;
        while (getline(m_stream, cur_line)) {
            size_t pos = cur_line.find(' ');
            auto cur_path = cur_line.substr(0, pos);
            auto cur_timestamp = std::stoi(cur_line.substr(pos + 1, cur_line.size()));
            callback(cur_path, cur_timestamp);
        }
    }

private:
    std::ifstream m_stream {};
};