#include "Utils.h"
#include <string>
#include <vector>

namespace Utils {

    std::vector<std::string> Split(const std::string& str, const std::string& del)
    {
        std::vector<std::string> result;
        size_t next_string_start = 0;
        for (size_t i = 0; i < str.size(); i++) {
            size_t j = i;
            while (j < str.size() && j - i < del.size() && str[j] == del[j - i]) {
                j++;
            }
            if (j - i == del.size()) {
                std::string res;
                for (int k = next_string_start; k < i; k++) {
                    res.push_back(str[k]);
                }
                result.push_back(move(res));
                i = j - 1;
                next_string_start = j;
            }
        }

        std::string res;
        for (int k = next_string_start; k < str.size(); k++) {
            res.push_back(str[k]);
        }
        result.push_back(move(res));

        return result;
    }

}