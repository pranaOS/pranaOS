#include <libpranaos/utils/formatter.h>
#include <libpranaos/utils/lexer.h>
#include <cstdio>

namespace utils {
    std::string formatter::format(std::string fmt, std::list<argument> args)
    { 
        std::string res;
        utils::lexer lexer_instance(fmt);

        while(lexer_instance)
        {
            if(lexer_instance.current() == '{')
            {
                if(lexer_instance.peek() == '{')
                {
                    res.append("{");

                    lexer_instance.advance();
                }
                else if(lexer_instance.peek() == '}')
                {
                    if(!args.empty())
                    {
                        argument arg = *args.begin();

                        if(arg.get_raw() == nullptr)
                        {
                            res.append("<null>");
                        }
                        else if(arg.is_type<std::string>() || arg.is_type<const std::string>())
                        {
                            res.append(arg.as<std::string>());
                        }
                        else if(arg.is_type<char*>() || arg.is_type<const char*>())
                        {
                            res.append(arg.as<const char*>());
                        }
                        else
                        {
                            res += "<unknown_type>";
                        }

                        args.pop_front();
                    }
                    else
                    {
                        res.append("<arg_absent>");
                    }
                }
                else
                {
                    res.append("<unterminated_expr>");
                }

                lexer_instance.advance();
            }
            else
            {
                char c = lexer_instance.current();
                res.append(std::string(1, c));
            }

            lexer_instance.advance();
        }

        return res;
    }
}