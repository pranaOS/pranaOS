#pragma once

#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class IncludeParser {
public:
    explicit IncludeParser(const std::filesystem::path& path)
    {
        m_stream.open(path, std::ifstream::in);
    }

    ~IncludeParser()
    {
        if (m_stream.is_open()) {
            m_stream.close();
        }
    }

    void run(const std::function<void(const std::string& include, bool global)>& callback)
    {
        std::string cur_line;
        while (getline(m_stream, cur_line)) {
            size_t i = 0;
            while (i < cur_line.size() && cur_line[i] == ' ') {
                i++;
            }
            cur_line = cur_line.substr(i, cur_line.size() - i);

            if (cur_line.empty()) {
                continue;
            }

            if (cur_line.starts_with("#pragma")) {
                continue;
            }

            if (!cur_line.starts_with("#include ")) {
                return;
            }

            bool global = true;
            if (cur_line[9] == '\"') {
                global = false;
            }

            cur_line = cur_line.substr(10, cur_line.size() - 10 - 1);

            callback(cur_line, global);
        }
    }

private:
    std::ifstream m_stream {};
};