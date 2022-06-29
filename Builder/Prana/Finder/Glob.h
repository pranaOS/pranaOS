#pragma once

#include <glob.h>
#include <cstring>
#include <filesystem>

class Glob {
    struct Fragment {
        std::filesystem::path path;
        bool recursive;
    };

public:
    explicit Glob(const std::string& pattern)
    {
        split_into_fragments(pattern);
        search();
    }

    std::vector<std::filesystem::path>&& result() { return std::move(m_result); }

private:
    inline void split_into_fragments(const std::string& pattern)
    {
        for (int at = 0; at < pattern.size(); at++) {
            if (at + 2 < pattern.size()) {
                if (pattern[at] == '*' && pattern[at + 1] == '/') {
                    m_fragments.push_back({ m_ending, false });
                    m_ending.clear();
                    at += 1;
                    continue;
                }
            }
            if (at + 3 < pattern.size()) {
                if (pattern[at] == '*' && pattern[at + 1] == '*' && pattern[at + 2] == '/') {
                    m_fragments.push_back({ m_ending, true });
                    m_ending.clear();
                    at += 2;
                    continue;
                }
            }
            m_ending.push_back(pattern[at]);
        }
    }

    inline void search(int fragment = 0, const std::filesystem::path& cur_path = "")
    {
        if (fragment == m_fragments.size()) {
            auto found = glob(cur_path / m_ending);
            std::copy(found.begin(), found.end(), std::back_inserter(m_result));
            return;
        }

        std::filesystem::path search_in = cur_path.empty() ? m_fragments[fragment].path : cur_path / m_fragments[fragment].path;
        search_in = search_in.empty() ? "." : search_in;

        if (!std::filesystem::exists(search_in)) {
            return;
        }

        if (m_fragments[fragment].recursive) {
            for (auto& file : std::filesystem::recursive_directory_iterator(search_in)) {
                search(fragment + 1, file.path());
            }
        } else {
            for (auto& file : std::filesystem::directory_iterator(search_in)) {
                search(fragment + 1, file.path());
            }
        }
    }

    static inline std::vector<std::filesystem::path> glob(const std::string& pattern) {
        glob_t glob_result;
        memset(&glob_result, 0, sizeof(glob_result));

        int res = ::glob(pattern.c_str(), GLOB_TILDE | GLOB_NOSORT, nullptr, &glob_result);
        std::vector<std::filesystem::path> filenames {};

        if (res >= 0) {
            for (size_t i = 0; i < glob_result.gl_pathc; i++) {
                filenames.emplace_back(glob_result.gl_pathv[i]);
            }
        }

        globfree(&glob_result);

        return filenames;
    }

private:
    std::vector<Fragment> m_fragments {};
    std::string m_ending {};
    std::vector<std::filesystem::path> m_result {};
};