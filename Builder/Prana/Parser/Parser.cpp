#include "Parser.h"

#include <functional>
#include <iostream>

#include "../Config.h"
#include "../Context.h"

Parser::Parser(const std::string& path, Context* context)
    : context(context)
{
    lexer = Lexer(path);
}

Parser::Parser(Parser&& parser) noexcept
{
    *this = std::move(parser);
}

Parser& Parser::operator=(Parser&& parser) noexcept
{
    lexer = std::move(parser.lexer);
    m_tokens_idx = parser.m_tokens_idx;
    context = parser.context;
    parser.context = nullptr;
    return *this;
}

void Parser::run()
{
    lexer.run();
    process_variables(false);

    while (auto token = lookup()) {
        if (token->type() != Token::Type::Default || !token->content_ptr()) {
            trigger_error_on_line(token->line(), "met unexpected token " + token->to_string());
        }
        if (token->content() == "Include") {
            parse_include();
            
            for (auto& path : context->m_include.paths()) {
                if (!context->run_as_childs(*path, Context::Operation::Parse)) {
                    trigger_error_on_line(token->line(), "Included path \"" + *path + "\" does not exist");
                }
            }
        } else if (token->content() == "Define") {
            parse_defines();
            process_variables(true);
        } else if (token->content() == "Commands") {
            parse_commands();
        } else if (token->content() == "Build") {
            parse_build();
        } else if (token->content() == "Default") {
            parse_default();
        } else {
            trigger_error_on_line(token->line(), "met unexpected token " + token->to_string());
            break;
        }
    }

    process_variables(true);
}

void Parser::parse_include()
{
    eat();
    eat_sub_rule_hard();

    parse_argument_list([this](const std::shared_ptr<std::string>& content) {
        context->m_include.add_path(content);
    });
}

void Parser::parse_defines()
{
    eat();
    eat_sub_rule_hard();

    std::function<void(int, bool)> parse_define_paris = [&](int nesting, bool save_result) {
        parse_line_by_line(nesting, [&](const Token& key_or_lhs) {
            if (lookup()->type() == Token::Type::SubRule) {
                eat();
                std::vector<std::shared_ptr<std::string>> defines;
                parse_argument_list_of_rule(*lookup(-1), [&](const std::shared_ptr<std::string>& content) {
                    if (save_result) {
                        defines.push_back(content);
                    }
                });
                if (save_result) {
                    context->m_defines.write_defines(key_or_lhs.content(), std::move(defines));
                }
                return;
            }

            if (lookup()->type() == Token::Type::Equal) {
                eat();
                auto rhs = eat();
                if (!rhs || rhs->line() != key_or_lhs.line()) {
                    trigger_error_on_line(key_or_lhs.line(), "no right hand sight for equal operation");
                }
                eat_sub_rule_hard();
                parse_define_paris(nesting + 1, Config::the().flags()[key_or_lhs.content()] == rhs->content());
                return;
            }

            trigger_error_on_line(key_or_lhs.line(), "incorrect define line");
        });
    };

    parse_define_paris(1, true);
}

void Parser::parse_commands()
{
    eat();
    eat_sub_rule_hard();

    parse_line_by_line(1, [this](const Token& cmd) {
        eat_sub_rule_hard();
        parse_argument_list([&](const std::shared_ptr<std::string>& content) {
            context->m_commands.append_to_command(cmd.content(), content);
        });
    });
}

void Parser::parse_build()
{
    eat();
    eat_sub_rule_hard();

    parse_line_by_line(1, [&](const Token& build_subfield) {
        if (build_subfield.content() == "Type") {
            eat_sub_rule_hard();
            auto type = parse_single_argument(build_subfield.line());
            if (!context->m_build.set_type(*type)) {
                trigger_error_on_line(build_subfield.line(), "incorrect type (choose either StaticLib or Executable)");
            }
        } else if (build_subfield.content() == "Depends") {
            eat_sub_rule_hard();
            parse_argument_list([&](const std::shared_ptr<std::string>& dependency) {
                context->m_build.add_dependency(dependency);
            });
            
            for (auto& dependency : context->m_build.depends()) {
                if (!context->run_as_childs(*dependency, Context::Operation::Build)) {
                    trigger_error_on_line(build_subfield.line(), "referenced dependency \"" + *dependency + "\" does not exist");
                }
            }
        } else if (build_subfield.content() == "HeaderFolders") {
            eat_sub_rule_hard();
            parse_argument_list([&](const std::shared_ptr<std::string>& header_folder) {
                context->m_build.add_header_folder(header_folder);
            });
        } else if (build_subfield.content() == "Src") {
            eat_sub_rule_hard();
            parse_argument_list([&](const std::shared_ptr<std::string>& source) {
                context->m_build.add_source(source);
            });
        } else if (build_subfield.content() == "Extensions") {
            eat_sub_rule_hard();
            parse_line_by_line(2, [&](const Token& extension) {
                eat_sub_rule_hard();

                bool options_specified = false;

                for (size_t i = 0; i < 2; i++) {
                    auto compiler_or_flag = parse_single_argument_of_rule(extension);
                    if (compiler_or_flag) {
                        options_specified = true;
                        if (*compiler_or_flag == "Compiler") {
                            eat_sub_rule_hard();
                            auto compiler = parse_single_argument(extension.line());
                            if (!compiler) {
                                trigger_error_on_line(extension.line(), " no compiler is specified");
                            }
                            if (!context->m_build.set_compiler_to_extension(extension.content_ptr(), compiler)) {
                                trigger_error_on_line(extension.line(), "compiler redefinition");
                            }
                        } else if (*compiler_or_flag == "Flags") {
                            eat_sub_rule_hard();
                            parse_argument_list([&](const std::shared_ptr<std::string>& flag) {
                                context->m_build.add_flag_to_extension(extension.content_ptr(), flag);
                            });
                        } else {
                            trigger_error_on_line(extension.line(), "invalid option for extension - " + *compiler_or_flag);
                        }
                    }
                }

                if (!options_specified) {
                    trigger_error_on_line(extension.line(), "no options specified for extension");
                }
            });
        } else if (build_subfield.content() == "Link") {
            eat_sub_rule_hard();

            parse_line_by_line(2, [this](const Token& linker_or_flags) {
                if (linker_or_flags.content() == "Linker") {
                    eat_sub_rule_hard();
                    auto linker = parse_single_argument_of_rule(linker_or_flags);
                    if (!linker) {
                        trigger_error_on_line(linker_or_flags.line(), "no linker is specified");
                    } else {
                        context->m_build.set_linker(linker);
                    }
                } else if (linker_or_flags.content() == "Flags") {
                    eat_sub_rule_hard();
                    parse_argument_list_of_rule(linker_or_flags, [this](const std::shared_ptr<std::string>& flag) {
                        context->m_build.add_linker_flag(flag);
                    });
                } else {
                    trigger_error_on_line(linker_or_flags.line(), "unknown Link option \"" + linker_or_flags.content() + "\"");
                }
            });
        } else if (build_subfield.content() == "Archiver") {
            eat_sub_rule_hard();
            auto archiver = parse_single_argument_of_rule(build_subfield);
            if (!archiver) {
                trigger_error_on_line(build_subfield.line(), "no Archiver is specified");
            } else {
                context->m_build.set_archiver(archiver);
            }
        } else {
            trigger_error_on_line(build_subfield.line(), "met unexpected Build subfield \"" + build_subfield.content() + "\"");
        }
    });
}

void Parser::parse_default()
{
    eat();
    eat_sub_rule_hard();

    parse_argument_list([this](const std::shared_ptr<std::string>& content) {
        context->m_default.add_command_to_sequence(content);
    });
}

void Parser::parse_line_by_line(size_t nesting, TokenProcessor process_token)
{
    while (lookup() && lookup(-1)->line() != lookup()->line() && lookup()->nesting() == nesting && lookup()->type() == Token::Type::Default) {
        process_token(*eat());
    }
}

std::shared_ptr<std::string> Parser::parse_single_argument(size_t line)
{
    std::shared_ptr<std::string> arg;
    parse_argument_list([&](const std::shared_ptr<std::string>& content) {
        if (arg) [[unlikely]] {
            trigger_error_on_line(line, "only one argument is allowed");
        }
        arg = content;
    });
    return arg;
}

std::shared_ptr<std::string> Parser::parse_single_argument_of_rule(const Token& rule)
{
    std::shared_ptr<std::string> arg;
    parse_argument_list_of_rule(rule, [&](const std::shared_ptr<std::string>& content) {
        if (arg) [[unlikely]] {
            trigger_error_on_line(rule.line(), "only one argument is allowed");
        }
        arg = content;
    });
    return arg;
}

void Parser::parse_argument_list(TokenContentProcessor process_content)
{
    parse_argument_list_of_rule(*lookup(-1), process_content);
}

void Parser::parse_argument_list_of_rule(const Token& rule, TokenContentProcessor process_content)
{
    size_t sub_rule_line = rule.line();
    int sub_rule_nesting = rule.nesting();

    if (!lookup()) {
        return;
    }

    if (sub_rule_line == lookup()->line()) {
        parse_lined_argument_list(sub_rule_line, process_content);
        return;
    }


    if (sub_rule_nesting < lookup()->nesting()) {
        int multiple_lines_nesting = lookup()->nesting();
        while (lookup() && (lookup()->type() == Token::Type::Default || lookup()->type() == Token::Type::Variable) && lookup()->nesting() == multiple_lines_nesting) {
            parse_lined_argument_list(lookup()->line(), process_content);
        }
    }
}

void Parser::parse_lined_argument_list(size_t line, TokenContentProcessor process_content)
{
    std::shared_ptr<std::string> arg = nullptr;

    while (lookup()) {
        if (lookup()->line() != line) {
            break;
        }

        if (lookup()->type() == Token::Type::Comma) {
            if (arg) {
                process_content(arg);
                arg = nullptr;
                eat();
            }
            continue;
        }

        if (lookup()->type() == Token::Type::Default) {
            if (!arg) {
                arg = eat()->content_ptr();
            } else {
                arg = std::make_shared<std::string>(*arg + eat()->content());
            }
            continue;
        }

        if (lookup()->type() == Token::Type::Variable) {
            auto variable_token = eat();
            auto& variable_name = variable_token->content();
            if (variable_name.empty()) {
                trigger_error_on_line(variable_token->line(), "variable has no name");
            }
            if (!context->m_defines.defines().contains(variable_name)) {
                trigger_error_on_line(variable_token->line(), "variable " + variable_name + " was not defined");
            }

            auto& define_list = context->m_defines.defines()[variable_name];

            for (size_t i = 0 ; i < define_list.size() ; i++) {
                auto& content = define_list[i];

                if (!arg) {
                    arg = content;
                } else {
                    arg = std::make_shared<std::string>(*arg + *content);
                }

                if (i < define_list.size() - 1) {
                    process_content(arg);
                    arg = nullptr;
                }
            }

            continue;
        }

        break;
    }

    if (arg) {
        process_content(arg);
    }
}

void Parser::process_variables(bool can_fail)
{
    return;
}
