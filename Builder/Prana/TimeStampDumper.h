#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

class TimeStampDumper {
public:
    void append(const std::string& path, int timestamp) 
    {
        m_stream << path << "" << timestamp << "\n";   
    }

private:
    std::ofstream m_stream {};
};