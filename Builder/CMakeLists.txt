cmake_minimum_required(VERSION 3.17)
project(pranaBuilder)

set(CMAKE_CXX_STANDARD 20)

add_executable(pranaBuilder Prana/main.cpp Prana/Parser/Lexer/Lexer.cpp Prana/Parser/Lexer/Lexer.h Prana/Parser/Lexer/Token.h Prana/Parser/Parser.cpp Prana/Parser/Parser.h Prana/Context.cpp Prana/Context.h Prana/Parser/Field/IncludeField.h Prana/Parser/Field/DefinesField.h Prana/Parser/Field/CommandsField.h Prana/Parser/Field/BuildField.h Prana/Parser/Field/DefaultField.h Prana/Finder/Finder.h Prana/Executor/Executor.cpp Prana/Executor/Executor.h Prana/Executor/Command.cpp Prana/Executor/Command.h Prana/Utils/Logger.h Prana/Utils/Utils.h Prana/Utils/Utils.cpp Prana/Utils/Utils.h Prana/Executor/ExecutableUnit.h Prana/Utils/ThreadQueue.h Prana/Utils/Lock.h Examples/wisteria/wisterialib/library.cpp Prana/Config.cpp Prana/Config.h Prana/Translator/Translator.cpp Prana/Translator/Translator.h Prana/Finder/Glob.h Prana/IncludeParser.h Prana/TimeStampParser.h Prana/TimeStampDumper.h)

file
(
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/
)