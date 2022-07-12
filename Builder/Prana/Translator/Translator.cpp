#include "Translator.h"
#include "../Context.h"
#include "../Finder/Finder.h"

#include <fstream>

void Translator::generate_cmake(Context* ctx, int a)
{
    bool library = ctx->build_field().type() == BuildField::Type::StaticLib;
    auto cmake_file = ctx->directory() / "CMakeLists.txt";

    std::fstream file(cmake_file, std::fstream::in | std::fstream::out | std::fstream::trunc);

    file << "cmake_minimum_required(VERSION 3.17)\n";
    if (ctx->root_ctx()) {
        file << "project(";
        file << ctx->name();
        file << ")\n";
    }
    file << "\n";

    for (const auto& extension : ctx->build_field().extensions()) {

        auto process_extension([&](auto& extension, auto& compiler_header, auto& flags_header) {
            file << "set(" << compiler_header << " \"" << *extension.second.compiler << "\")\n";

            std::string flags;
            auto it = extension.second.flags.begin();
            while (it != extension.second.flags.end()) {
                flags.append(**it);
                if (++it != extension.second.flags.end()) {
                    flags.append(" ");
                }
            }
            file << "set(" << flags_header << " \"" << flags << "\")\n";
        });

        if (extension.first == "cpp") {
            process_extension(extension, "CMAKE_CXX_COMPILER", "CMAKE_CXX_FLAGS");
        } else if (extension.first == "c") {
            process_extension(extension, "CMAKE_C_COMPILER", "CMAKE_C_FLAGS");
        }
    }

    file << "\n";

    // Process Linker Flags
    if (!library) {

    }

    bool met_static_lib = false;

    for (auto child_ctx : ctx->children()) {
        if (child_ctx->build_field().type() == BuildField::Type::StaticLib) {
            met_static_lib = true;
        }
        auto subdir = std::filesystem::proximate(child_ctx->directory(), ctx->directory()).string();
        file << "add_subdirectory(\"" << subdir << "\")\n";
    }
    file << "\n";

    file << (library ? "add_library" : "add_executable") << "(";
    file << ctx->name();
    for (auto& source : ctx->build_field().sources()) {
        auto files = Finder::FindFiles(ctx->directory(), *source);
        for (auto& filename : files) {
            file << " " << std::filesystem::proximate(filename, ctx->directory()).string();
        }
    }
    file << ")\n\n";

    if (met_static_lib) {
        file << "target_link_libraries(" << ctx->name();

        for (auto child_ctx : ctx->children()) {
            if (child_ctx->build_field().type() == BuildField::Type::StaticLib) {
                auto subdir = std::filesystem::proximate(child_ctx->directory(), ctx->directory()).string();
                file << " " << subdir;
            }
        }

        file << ")\n";
    }

    Log(Color::Magenta, "Generated:", cmake_file.string());
}