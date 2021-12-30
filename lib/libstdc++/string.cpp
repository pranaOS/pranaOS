#include <string>
#include <cstdio>
#include <cstdint>

namespace std
{
    template <typename T>
    static T to_the_power(T base, uint32_t expo)
    {
        if(expo == 0)
        {
            return 1;
        }
        else if((expo % 2) == 0)
        {
            return to_the_power(base, expo / 2) * to_the_power(base, expo / 2);
        }

        return base * to_the_power(base, expo / 2) * to_the_power(base, expo / 2);
    }

    template <typename T>
    static size_t count_digits(T x, int base)  
    {
        x = x >= 0 ? x : -x;

        int digits = 0;

        do
        {
            x /= base;
            digits++;
        }
        while(x != 0);

        return digits;
    }

    template <typename T>
    static std::string int_to_str(T x, int base, bool use_uppercase = false)
    {
        std::string res;

        if(x < 0)
        {
            res.append("-");
            x = -x;
        }

        for(int i = count_digits(x, base) - 1; i >= 0; i--)
        {
            int digit = x / to_the_power<T>(base, i) % base;

            res.append(1, (use_uppercase ? "0123456789ABCDEFGHIJKLNMOPQRSTUVWXYZ" : "0123456789abcdefghijklnmopqrstuvwxyz")[digit]);
        }

        return res;
    }

    std::string to_string(int32_t value)
    {
        return int_to_str(value, 10);
    }

    std::string to_string(uint32_t value)
    {
        return int_to_str(value, 10);
    }

    std::string to_string(int64_t value)
    {
        return int_to_str(value, 10);
    }

    std::string to_string(uint64_t value)
    {
        return int_to_str(value, 10);
    }
}