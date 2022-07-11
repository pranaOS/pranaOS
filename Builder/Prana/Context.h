//
//  Context.h
//  Prana
//
//  Created by Pranav on 11/07/22.
//

#pragma once

#include <filesystem>

constexpr size_t path_max = 4096;

enum class IncludeStatus {
    NotVisited,
    NeedsRecompilation,
    UpToDate,
};

class Context {
    friend class Parser;
    friend class Executor;
    
public:
    enum class State {
        NotStarted,
        ParserError,
        Parsed,
        BuiltError,
    }
    
    enum class Operation {
        Parse,
        Build,
    };
    
public:
    Context(std::filesystem::path path, Operation operation, const DefinesField& defines = {}, bool root_ctx = false);
    
    void run();
}
