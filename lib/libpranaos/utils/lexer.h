#pragma once

#include <string>

namespace utils {
    class lexer {
    private:
        std::string _source = {};
        int _index = 0;
        bool _eof = false;

    public:
        lexer(const std::string source)
        :
        _source(source)
        {
        }
        
        lexer()
        {
        }

        std::string& source() { return _source; }
        int get_index() { return _index; }

        void load(const std::string& source) { _source = source; }

        char peek(int how_many = 1) const
        {
            int i = _index + how_many;
            if (i >= _source.length() || i < 0) return '\0';

            return _source[i];
        }
        
        char current() const
        {
            return _source[_index];
        }

        bool is_eof();

        explicit operator bool() {
            return !is_eof();
        }
    };
}