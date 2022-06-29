#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class BuildField {
public:
    enum class Type {
        Unknown,
        StaticLib,
        Executable,
    };

    struct ExtensionOption {
        std::shared_ptr<std::string> compiler;
        std::vector<std::shared_ptr<std::string>> flags {};
    };

public:
    BuildField() = default;

    inline bool set_type(const std::string& type)
    {
        if (type == "StaticLib") {
            m_type = Type::StaticLib;
        } else if (type == "Executable") {
            m_type = Type::Executable;
        } else {
            return false;
        }
        return true;
    }

    inline void add_dependency(const std::shared_ptr<std::string>& dependency)
    {
        m_depends.push_back(dependency);
    }

    inline void add_header_folder(const std::shared_ptr<std::string>& header_folder)
    {
        m_header_folders.push_back(header_folder);
    }

    inline void add_source(const std::shared_ptr<std::string>& source)
    {
        m_sources.push_back(source);
    }

    inline bool set_compiler_to_extension(const std::shared_ptr<std::string>& extension, const std::shared_ptr<std::string>& compiler)
    {
        if (m_extensions[*extension].compiler) [[unlikely]] {
            return false;
        }
        m_extensions[*extension].compiler = compiler;
        return true;
    }

    inline void add_flag_to_extension(const std::shared_ptr<std::string>& extension, const std::shared_ptr<std::string>& flag)
    {
        m_extensions[*extension].flags.push_back(flag);
    }

    void set_linker(const std::shared_ptr<std::string>& linker) { m_linker = linker; }
    void set_archiver(const std::shared_ptr<std::string>& archiver) { m_archiver = archiver; }
    void add_linker_flag(const std::shared_ptr<std::string>& flag) { m_linker_flags.push_back(flag); }

    inline Type type() const { return m_type; }
    inline const auto& depends() const { return m_depends; }
    inline const auto& header_folders() const { return m_header_folders; }
    inline const auto& sources() const { return m_sources; }
    auto& linker() { return m_linker; }
    auto& archiver() { return m_archiver; }
    auto& linker_flags() { return m_linker_flags; }
    const auto& extensions() const { return m_extensions; }

    inline ExtensionOption* get_option_for_file(const std::string& file)
    {
        for (auto& extension : m_extensions) {
            if (file.ends_with(extension.first)) {
                return &extension.second;
            }
        }
        return nullptr;
    }

private:
    Type m_type {};
    std::vector<std::shared_ptr<std::string>> m_depends {};
    std::vector<std::shared_ptr<std::string>> m_header_folders {};
    std::vector<std::shared_ptr<std::string>> m_sources {};
    std::unordered_map<std::string, ExtensionOption> m_extensions {};

    std::shared_ptr<std::string> m_archiver {};
    std::shared_ptr<std::string> m_linker {};
    std::vector<std::shared_ptr<std::string>> m_linker_flags {};
};