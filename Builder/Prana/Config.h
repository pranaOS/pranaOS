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
    static inline auto& the()
    {
        static auto instance = Config();
        return instance;
    }

public:
    void process_arguments(int argc, char** argv);

public:
    const auto& filename() const { return m_filename; }
    const auto& arguments() const { return m_arguments; }
    auto& flags()  { return m_flags; }
    Mode mode() const { return m_mode; }
    int timestamp() const { return m_timestamp; }

private:
    Config();

private:
    int m_timestamp;
    std::string m_filename {};
    std::vector<std::string> m_arguments {};
    std::unordered_map<std::string, std::string> m_flags {};
    Mode m_mode {};
};