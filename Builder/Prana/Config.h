#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class Config {
public:
    enum class Mode {
        Generate,
        Default,
        CommandList,
    };

public:
    static inline auto& the ()
    {
        static auto instance = Config();
        return instance;
    }

public:
    void process_arguments(int argc, char** argv);

private:
    Config();

private:
    int m_timestamp;
    std::string m_filename {};
    Mode m_mode {};
};