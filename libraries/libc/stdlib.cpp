/**
 * @file stdlib.cpp
 * @author Krisna Pranav
 * @brief stdlib
 * @version 6.0
 * @date 2023-08-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */


#include <mods/assertions.h>
#include <mods/hashmap.h>
#include <mods/noncopyable.h>
#include <mods/random.h>
#include <mods/stdlibextra.h>
#include <mods/types.h>
#include <mods/utf8view.h>
#include <libelf/auxiliaryvector.h>
#include <libpthread/pthread.h>
#include <alloca.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/internals.h>
#include <sys/ioctl.h>
#include <sys/ioctl_numbers.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/wait.h>
#include <syscall.h>
#include <unistd.h>
#include <wchar.h>

/**
 * @param str 
 * @param endptr 
 */
static void strtons(char const* str, char** endptr)
{
    assert(endptr);
    char* ptr = const_cast<char*>(str);
    while (isspace(*ptr)) {
        ptr += 1;
    }
    *endptr = ptr;
}

enum Sign {
    Negative,
    Positive,
}; // enum Sign

/**
 * @param str 
 * @param endptr 
 * @return Sign 
 */
static Sign strtosign(char const* str, char** endptr)
{
    assert(endptr);
    if (*str == '+') {
        *endptr = const_cast<char*>(str + 1);
        return Sign::Positive;
    } else if (*str == '-') {
        *endptr = const_cast<char*>(str + 1);
        return Sign::Negative;
    } else {
        *endptr = const_cast<char*>(str);
        return Sign::Positive;
    }
}

enum DigitConsumeDecision {
    Consumed,
    PosOverflow,
    NegOverflow,
    Invalid,
}; // enum DigitConsumeDecision

/**
 * @tparam T 
 * @tparam min_value 
 * @tparam max_value 
 */
template<typename T, T min_value, T max_value>
class NumParser 
{
    MOD_MAKE_NONMOVABLE(NumParser);

public:
    /**
     * @brief Construct a new Num Parser object
     * 
     * @param sign 
     * @param base 
     */
    NumParser(Sign sign, int base)
        : m_base(base)
        , m_num(0)
        , m_sign(sign)
    {
        m_cutoff = positive() ? (max_value / base) : (min_value / base);
        m_max_digit_after_cutoff = positive() ? (max_value % base) : (min_value % base);
    }

    /**
     * @param ch 
     * @return int 
     */
    int parse_digit(char ch)
    {
        int digit;
        if (isdigit(ch))
            digit = ch - '0';
        else if (islower(ch))
            digit = ch - ('a' - 10);
        else if (isupper(ch))
            digit = ch - ('A' - 10);
        else
            return -1;

        if (static_cast<T>(digit) >= m_base)
            return -1;

        return digit;
    }

    /**
     * @param ch 
     * @return DigitConsumeDecision 
     */
    DigitConsumeDecision consume(char ch)
    {
        int digit = parse_digit(ch);
        if (digit == -1)
            return DigitConsumeDecision::Invalid;

        if (!can_append_digit(digit)) {
            if (m_sign != Sign::Negative) {
                return DigitConsumeDecision::PosOverflow;
            } else {
                return DigitConsumeDecision::NegOverflow;
            }
        }

        m_num *= m_base;
        m_num += positive() ? digit : -digit;

        return DigitConsumeDecision::Consumed;
    }

    /**
     * @return T 
     */
    T number() const 
    { 
        return m_num; 
    };

private:
    /**
     * @param digit 
     * @return true 
     * @return false 
     */
    bool can_append_digit(int digit)
    {
        bool const is_below_cutoff = positive() ? (m_num < m_cutoff) : (m_num > m_cutoff);

        if (is_below_cutoff) {
            return true;
        } else {
            return m_num == m_cutoff && digit < m_max_digit_after_cutoff;
        }
    }

    bool positive() const
    {
        return m_sign != Sign::Negative;
    }

    const T m_base;
    T m_num;
    T m_cutoff;
    int m_max_digit_after_cutoff;
    Sign m_sign;
}; // class NumParser

typedef NumParser<int, INT_MIN, INT_MAX> IntParser;
typedef NumParser<long long, LONG_LONG_MIN, LONG_LONG_MAX> LongLongParser;
typedef NumParser<unsigned long long, 0ULL, ULONG_LONG_MAX> ULongLongParser;

/**
 * @param str 
 * @param offset 
 * @param lower 
 * @param upper 
 * @return true 
 * @return false 
 */
static bool is_either(char* str, int offset, char lower, char upper)
{
    char ch = *(str + offset);
    return ch == lower || ch == upper;
}

/**
 * @tparam Callback 
 * @param pattern 
 * @param callback 
 * @return int 
 */
template<typename Callback>
inline int generate_unique_filename(char* pattern, Callback callback)
{
    size_t length = strlen(pattern);

    if (length < 6 || memcmp(pattern + length - 6, "XXXXXX", 6))
        return EINVAL;

    size_t start = length - 6;

    constexpr char random_characters[] = "abcdefghijklmnopqrstuvwxyz0123456789";

    for (int attempt = 0; attempt < 100; ++attempt) {
        for (int i = 0; i < 6; ++i)
            pattern[start + i] = random_characters[(arc4random() % (sizeof(random_characters) - 1))];
        if (callback() == IterationDecision::Break)
            return 0;
    }

    return EEXIST;
}

extern "C" {

/**
 * @param status 
 */
void exit(int status)
{
    __cxa_finalize(nullptr);

    if (secure_getenv("LIBC_DUMP_MALLOC_STATS"))
        pranaos_dump_malloc_stats();

    extern void _fini();
    _fini();
    fflush(nullptr);

#ifndef _DYNAMIC_LOADER
    __pthread_key_destroy_for_current_thread();
#endif

    _exit(status);
}

/**
 * @param handler 
 */
static void __atexit_to_cxa_atexit(void* handler)
{
    reinterpret_cast<void (*)()>(handler)();
}

/**
 * @param handler 
 * @return int 
 */
int atexit(void (*handler)())
{
    return __cxa_atexit(__atexit_to_cxa_atexit, (void*)handler, nullptr);
}

void _abort()
{
    asm volatile("ud2");
    __builtin_unreachable();
}

void abort()
{
    raise(SIGABRT);

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGABRT);
    sigprocmask(SIG_UNBLOCK, &set, nullptr);
    raise(SIGABRT);
    _abort();
}

static HashTable<FlatPtr> s_malloced_environment_variables;

/**
 * @param var 
 */
static void free_environment_variable_if_needed(char const* var)
{
    if (!s_malloced_environment_variables.contains((FlatPtr)var))
        return;
    free(const_cast<char*>(var));
    s_malloced_environment_variables.remove((FlatPtr)var);
}

/**
 * @param name 
 * @return char* 
 */
char* getenv(char const* name)
{
    size_t vl = strlen(name);
    for (size_t i = 0; environ[i]; ++i) {
        char const* decl = environ[i];
        char* eq = strchr(decl, '=');
        if (!eq)
            continue;
        size_t varLength = eq - decl;
        if (vl != varLength)
            continue;
        if (strncmp(decl, name, varLength) == 0) {
            return eq + 1;
        }
    }
    return nullptr;
}

/**
 * @param name 
 * @return char* 
 */
char* secure_getenv(char const* name)
{
    if (getauxval(AT_SECURE))
        return nullptr;
    return getenv(name);
}

/**
 * @param name 
 * @return int 
 */
int unsetenv(char const* name)
{
    auto new_var_len = strlen(name);
    size_t environ_size = 0;
    int skip = -1;

    for (; environ[environ_size]; ++environ_size) {
        char* old_var = environ[environ_size];
        char* old_eq = strchr(old_var, '=');
        VERIFY(old_eq);
        size_t old_var_len = old_eq - old_var;

        if (new_var_len != old_var_len)
            continue; 

        if (strncmp(name, old_var, new_var_len) == 0)
            skip = environ_size;
    }

    if (skip == -1)
        return 0; 

    memmove(&environ[skip], &environ[skip + 1], ((environ_size - 1) - skip) * sizeof(environ[0]));
    environ[environ_size - 1] = nullptr;

    free_environment_variable_if_needed(name);
    return 0;
}

int clearenv()
{
    size_t environ_size = 0;
    for (; environ[environ_size]; ++environ_size) {
        environ[environ_size] = NULL;
    }
    *environ = NULL;
    return 0;
}

/**
 * @param name 
 * @param value 
 * @param overwrite 
 * @return int 
 */
int setenv(char const* name, char const* value, int overwrite)
{
    return pranaos_setenv(name, strlen(name), value, strlen(value), overwrite);
}

/**
 * @param name 
 * @param name_length 
 * @param value 
 * @param value_length 
 * @param overwrite 
 * @return int 
 */
int pranaos_setenv(char const* name, ssize_t name_length, char const* value, ssize_t value_length, int overwrite)
{
    if (!overwrite && getenv(name))
        return 0;

    auto const total_length = name_length + value_length + 2;
    auto* var = (char*)malloc(total_length);
    snprintf(var, total_length, "%s=%s", name, value);
    s_malloced_environment_variables.set((FlatPtr)var);
    return putenv(var);
}

/**
 * @param new_var 
 * @return int 
 */
int putenv(char* new_var)
{
    char* new_eq = strchr(new_var, '=');

    if (!new_eq)
        return unsetenv(new_var);

    auto new_var_len = new_eq - new_var;
    int environ_size = 0;
    for (; environ[environ_size]; ++environ_size) {
        char* old_var = environ[environ_size];
        char* old_eq = strchr(old_var, '=');
        VERIFY(old_eq);
        auto old_var_len = old_eq - old_var;

        if (new_var_len != old_var_len)
            continue; 

        if (strncmp(new_var, old_var, new_var_len) == 0) {
            free_environment_variable_if_needed(old_var);
            environ[environ_size] = new_var;
            return 0;
        }
    }

    auto** new_environ = static_cast<char**>(kmalloc_array(environ_size + 2, sizeof(char*)));
    if (new_environ == nullptr) {
        errno = ENOMEM;
        return -1;
    }

    for (int i = 0; environ[i]; ++i) {
        new_environ[i] = environ[i];
    }

    new_environ[environ_size] = new_var;
    new_environ[environ_size + 1] = nullptr;

    extern bool __environ_is_malloced;
    if (__environ_is_malloced)
        free(environ);
    __environ_is_malloced = true;
    environ = new_environ;
    return 0;
}

static char const* __progname = NULL;

char const* getprogname()
{
    return __progname;
}

/**
 * @param progname 
 */
void setprogname(char const* progname)
{
    for (int i = strlen(progname) - 1; i >= 0; i--) {
        if (progname[i] == '/') {
            __progname = progname + i + 1;
            return;
        }
    }

    __progname = progname;
}

/**
 * @param str 
 * @param endptr 
 * @return double 
 */
double strtod(char const* str, char** endptr)
{
    char* parse_ptr = const_cast<char*>(str);
    strtons(parse_ptr, &parse_ptr);
    const Sign sign = strtosign(parse_ptr, &parse_ptr);

    if (is_either(parse_ptr, 0, 'i', 'I')) {
        if (is_either(parse_ptr, 1, 'n', 'N')) {
            if (is_either(parse_ptr, 2, 'f', 'F')) {
                parse_ptr += 3;
                if (is_either(parse_ptr, 0, 'i', 'I')) {
                    if (is_either(parse_ptr, 1, 'n', 'N')) {
                        if (is_either(parse_ptr, 2, 'i', 'I')) {
                            if (is_either(parse_ptr, 3, 't', 'T')) {
                                if (is_either(parse_ptr, 4, 'y', 'Y')) {
                                    parse_ptr += 5;
                                }
                            }
                        }
                    }
                }
                if (endptr)
                    *endptr = parse_ptr;

                if (sign != Sign::Negative) {
                    return __builtin_huge_val();
                } else {
                    return -__builtin_huge_val();
                }
            }
        }
    }
    if (is_either(parse_ptr, 0, 'n', 'N')) {
        if (is_either(parse_ptr, 1, 'a', 'A')) {
            if (is_either(parse_ptr, 2, 'n', 'N')) {
                if (endptr)
                    *endptr = parse_ptr + 3;
                errno = ERANGE;
                if (sign != Sign::Negative) {
                    return __builtin_nan("");
                } else {
                    return -__builtin_nan("");
                }
            }
        }
    }

    char exponent_lower;
    char exponent_upper;
    int base = 10;
    if (*parse_ptr == '0') {
        char const base_ch = *(parse_ptr + 1);
        if (base_ch == 'x' || base_ch == 'X') {
            base = 16;
            parse_ptr += 2;
        }
    }

    if (base == 10) {
        exponent_lower = 'e';
        exponent_upper = 'E';
    } else {
        exponent_lower = 'p';
        exponent_upper = 'P';
    }

    LongLongParser digits { sign, base };
    bool digits_usable = false;
    bool should_continue = true;
    bool digits_overflow = false;
    bool after_decimal = false;
    int exponent = 0;
    do {
        if (!after_decimal && *parse_ptr == '.') {
            after_decimal = true;
            parse_ptr += 1;
            continue;
        }

        bool is_a_digit;
        if (digits_overflow) {
            is_a_digit = digits.parse_digit(*parse_ptr) != -1;
        } else {
            DigitConsumeDecision decision = digits.consume(*parse_ptr);
            switch (decision) {
            case DigitConsumeDecision::Consumed:
                is_a_digit = true;
                digits_usable = true;
                break;
            case DigitConsumeDecision::PosOverflow:
            case DigitConsumeDecision::NegOverflow:
                is_a_digit = true;
                digits_overflow = true;
                break;
            case DigitConsumeDecision::Invalid:
                is_a_digit = false;
                break;
            default:
                VERIFY_NOT_REACHED();
            }
        }

        if (is_a_digit) {
            exponent -= after_decimal ? 1 : 0;
            exponent += digits_overflow ? 1 : 0;
        }

        should_continue = is_a_digit;
        parse_ptr += should_continue;
    } while (should_continue);

    if (!digits_usable) {
        if (endptr)
            *endptr = const_cast<char*>(str);
        return 0.0;
    }

    if (*parse_ptr == exponent_lower || *parse_ptr == exponent_upper) {
        char* old_parse_ptr = parse_ptr;
        parse_ptr += 1;

        Sign exponent_sign = strtosign(parse_ptr, &parse_ptr);
        IntParser exponent_parser { exponent_sign, base };
        bool exponent_usable = false;
        bool exponent_overflow = false;
        should_continue = true;
        do {
            bool is_a_digit;
            if (exponent_overflow) {
                is_a_digit = exponent_parser.parse_digit(*parse_ptr) != -1;
            } else {
                DigitConsumeDecision decision = exponent_parser.consume(*parse_ptr);
                switch (decision) {
                case DigitConsumeDecision::Consumed:
                    is_a_digit = true;
                    exponent_usable = true;
                    break;
                case DigitConsumeDecision::PosOverflow:
                case DigitConsumeDecision::NegOverflow:
                    is_a_digit = true;
                    exponent_overflow = true;
                    break;
                case DigitConsumeDecision::Invalid:
                    is_a_digit = false;
                    break;
                default:
                    VERIFY_NOT_REACHED();
                }
            }

            should_continue = is_a_digit;
            parse_ptr += should_continue;
        } while (should_continue);

        if (!exponent_usable) {
            parse_ptr = old_parse_ptr;
        } else if (exponent_overflow) {
            if (sign != Sign::Negative) {
                exponent = INT_MIN;
            } else {
                exponent = INT_MAX;
            }
        } else {
            long long new_exponent = static_cast<long long>(exponent) + static_cast<long long>(exponent_parser.number());
            if (new_exponent < INT_MIN) {
                exponent = INT_MIN;
            } else if (new_exponent > INT_MAX) {
                exponent = INT_MAX;
            } else {
                exponent = static_cast<int>(new_exponent);
            }
        }
    }

    if (endptr)
        *endptr = const_cast<char*>(parse_ptr);

    if (digits.number() == 0) {
        if (sign != Sign::Negative) {
            return 0.0;
        } else {
            return -0.0;
        }
    }

    if (exponent <= -344) {
        errno = ERANGE;

        if (sign != Sign::Negative) {
            return 0.0;
        } else {
            return -0.0;
        }
    }

    if (exponent >= 309) {
        errno = ERANGE;

        if (sign != Sign::Negative) {
            return __builtin_huge_val();
        } else {
            return -__builtin_huge_val();
        }
    }

    double value = digits.number();
    double scale = 1;

    if (exponent < 0) {
        exponent = -exponent;
        for (int i = 0; i < min(exponent, 300); ++i) {
            scale *= base;
        }
        value /= scale;
        for (int i = 300; i < exponent; i++) {
            value /= base;
        }
        if (value == -0.0 || value == +0.0) {
            errno = ERANGE;
        }
    } else if (exponent > 0) {
        for (int i = 0; i < exponent; ++i) {
            scale *= base;
        }
        value *= scale;
        if (value == -__builtin_huge_val() || value == +__builtin_huge_val()) {
            errno = ERANGE;
        }
    }

    return value;
}

/**
 * @param str 
 * @param endptr 
 * @return long double 
 */
long double strtold(char const* str, char** endptr)
{
    assert(sizeof(double) == sizeof(long double));
    return strtod(str, endptr);
}

/**
 * @param str 
 * @param endptr 
 * @return float 
 */
float strtof(char const* str, char** endptr)
{
    return strtod(str, endptr);
}

/**
 * @param str 
 * @return double 
 */
double atof(char const* str)
{
    return strtod(str, nullptr);
}

/**
 * @param str 
 * @return int 
 */
int atoi(char const* str)
{
    long value = strtol(str, nullptr, 10);
    if (value > INT_MAX) {
        return INT_MAX;
    }
    return value;
}

/**
 * @param str 
 * @return long 
 */
long atol(char const* str)
{
    return strtol(str, nullptr, 10);
}

/**
 * @param str 
 * @return long long 
 */
long long atoll(char const* str)
{
    return strtoll(str, nullptr, 10);
}

static char ptsname_buf[32];

/**
 * @param fd 
 * @return char* 
 */
char* ptsname(int fd)
{
    if (ptsname_r(fd, ptsname_buf, sizeof(ptsname_buf)) < 0)
        return nullptr;
    return ptsname_buf;
}

/**
 * @param fd 
 * @param buffer 
 * @param size 
 * @return int 
 */
int ptsname_r(int fd, char* buffer, size_t size)
{
    struct stat stat;
    if (fstat(fd, &stat) < 0)
        return -1;

    StringBuilder devpts_path_builder;
    devpts_path_builder.append("/dev/pts/"sv);

    int master_pty_index = 0;

    if (ioctl(fd, TIOCGPTN, &master_pty_index) < 0)
        return -1;

    if (master_pty_index < 0) {
        errno = EINVAL;
        return -1;
    }

    devpts_path_builder.appendff("{:d}", master_pty_index);
    if (devpts_path_builder.length() > size) {
        errno = ERANGE;
        return -1;
    }
    memset(buffer, 0, devpts_path_builder.length() + 1);
    auto full_devpts_path_string = devpts_path_builder.build();
    if (!full_devpts_path_string.copy_characters_to_buffer(buffer, size)) {
        errno = ERANGE;
        return -1;
    }
    return 0;
}

static unsigned long s_next_rand = 1;

int rand()
{
    s_next_rand = s_next_rand * 1103515245 + 12345;
    return ((unsigned)(s_next_rand / ((RAND_MAX + 1) * 2)) % (RAND_MAX + 1));
}

/**
 * @param seed 
 */
void srand(unsigned seed)
{
    s_next_rand = seed;
}

/**
 * @param i 
 * @return int 
 */
int abs(int i)
{
    return i < 0 ? -i : i;
}

/**
 * @param i 
 * @return long int 
 */
long int labs(long int i)
{
    return i < 0 ? -i : i;
}

/**
 * @param i 
 * @return long long int 
 */
long long int llabs(long long int i)
{
    return i < 0 ? -i : i;
}

long int random()
{
    return rand();
}

/**
 * @param seed 
 */
void srandom(unsigned seed)
{
    srand(seed);
}

/**
 * @param command 
 * @return int 
 */
int system(char const* command)
{
    if (!command)
        return 1;

    pid_t child;
    char const* argv[] = { "sh", "-c", command, nullptr };
    if ((errno = posix_spawn(&child, "/bin/sh", nullptr, nullptr, const_cast<char**>(argv), environ)))
        return -1;
    int wstatus;
    waitpid(child, &wstatus, 0);
    return WEXITSTATUS(wstatus);
}

/**
 * @param pattern 
 * @return char* 
 */
char* mktemp(char* pattern)
{
    auto error = generate_unique_filename(pattern, [&] {
        struct stat st;
        int rc = lstat(pattern, &st);
        if (rc < 0 && errno == ENOENT)
            return IterationDecision::Break;
        return IterationDecision::Continue;
    });
    if (error) {
        pattern[0] = '\0';
        errno = error;
    }
    return pattern;
}

/**
 * @param pattern 
 * @return int 
 */
int mkstemp(char* pattern)
{
    int fd = -1;
    auto error = generate_unique_filename(pattern, [&] {
        fd = open(pattern, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR); // I'm using the flags I saw glibc using.
        if (fd >= 0)
            return IterationDecision::Break;
        return IterationDecision::Continue;
    });
    if (error) {
        errno = error;
        return -1;
    }
    return fd;
}

/**
 * @param pattern 
 * @return char* 
 */
char* mkdtemp(char* pattern)
{
    auto error = generate_unique_filename(pattern, [&] {
        if (mkdir(pattern, 0700) == 0)
            return IterationDecision::Break;
        return IterationDecision::Continue;
    });
    if (error) {
        errno = error;
        return nullptr;
    }
    return pattern;
}

/**
 * @param key 
 * @param base 
 * @param nmemb 
 * @param size 
 * @param compar 
 * @return void* 
 */
void* bsearch(void const* key, void const* base, size_t nmemb, size_t size, int (*compar)(void const*, void const*))
{
    char* start = static_cast<char*>(const_cast<void*>(base));
    while (nmemb > 0) {
        char* middle_memb = start + (nmemb / 2) * size;
        int comparison = compar(key, middle_memb);
        if (comparison == 0)
            return middle_memb;
        else if (comparison > 0) {
            start = middle_memb + size;
            --nmemb;
        }
        nmemb /= 2;
    }

    return nullptr;
}

/**
 * @param numerator 
 * @param denominator 
 * @return div_t 
 */
div_t div(int numerator, int denominator)
{
    div_t result;
    result.quot = numerator / denominator;
    result.rem = numerator % denominator;

    if (numerator >= 0 && result.rem < 0) {
        result.quot++;
        result.rem -= denominator;
    }
    return result;
}

/**
 * @param numerator 
 * @param denominator 
 * @return ldiv_t 
 */
ldiv_t ldiv(long numerator, long denominator)
{
    ldiv_t result;
    result.quot = numerator / denominator;
    result.rem = numerator % denominator;

    if (numerator >= 0 && result.rem < 0) {
        result.quot++;
        result.rem -= denominator;
    }
    return result;
}

/**
 * @param numerator 
 * @param denominator 
 * @return lldiv_t 
 */
lldiv_t lldiv(long long numerator, long long denominator)
{
    lldiv_t result;
    result.quot = numerator / denominator;
    result.rem = numerator % denominator;

    if (numerator >= 0 && result.rem < 0) {
        result.quot++;
        result.rem -= denominator;
    }
    return result;
}

/**
 * @param s 
 * @param n 
 * @return int 
 */
int mblen(char const* s, size_t n)
{
    static mbstate_t internal_state = {};

    if (s == nullptr) {
        internal_state = {};
        return 0;
    }

    size_t ret = mbrtowc(nullptr, s, n, &internal_state);

    if (ret == -2ul) {
        errno = EILSEQ;
        return -1;
    }

    return ret;
}

/**
 * @param pwcs 
 * @param s 
 * @param n 
 * @return size_t 
 */
size_t mbstowcs(wchar_t* pwcs, char const* s, size_t n)
{
    static mbstate_t state = {};
    return mbsrtowcs(pwcs, &s, n, &state);
}

/**
 * @param pwc 
 * @param s 
 * @param n 
 * @return int 
 */
int mbtowc(wchar_t* pwc, char const* s, size_t n)
{
    static mbstate_t internal_state = {};

    if (s == nullptr) {
        internal_state = {};
        return 0;
    }

    size_t ret = mbrtowc(pwc, s, n, &internal_state);

    if (ret == -2ul) {
        errno = EILSEQ;
        return -1;
    }

    return ret;
}

/**
 * @param s 
 * @param wc 
 * @return int 
 */
int wctomb(char* s, wchar_t wc)
{
    static mbstate_t _internal_state = {};

    if (s == nullptr)
        return 0;

    return static_cast<int>(wcrtomb(s, wc, &_internal_state));
}

/**
 * @param dest 
 * @param src 
 * @param max 
 * @return size_t 
 */
size_t wcstombs(char* dest, wchar_t const* src, size_t max)
{
    char* original_dest = dest;
    while ((size_t)(dest - original_dest) < max) {
        StringView v { (char const*)src, sizeof(wchar_t) };

        Utf8View utf8 { v };
        if (*utf8.begin() == '\0') {
            *dest = '\0';
            return (size_t)(dest - original_dest); 
        }

        for (auto byte : utf8) {
            if (byte != '\0')
                *dest++ = byte;
        }
        ++src;
    }
    return max;
}

/**
 * @param str 
 * @param endptr 
 * @param base 
 * @return long 
 */
long strtol(char const* str, char** endptr, int base)
{
    long long value = strtoll(str, endptr, base);
    if (value < LONG_MIN) {
        errno = ERANGE;
        return LONG_MIN;
    } else if (value > LONG_MAX) {
        errno = ERANGE;
        return LONG_MAX;
    }
    return value;
}

/**
 * @param str 
 * @param endptr 
 * @param base 
 * @return unsigned long 
 */
unsigned long strtoul(char const* str, char** endptr, int base)
{
    unsigned long long value = strtoull(str, endptr, base);
    if (value > ULONG_MAX) {
        errno = ERANGE;
        return ULONG_MAX;
    }
    return value;
}

/**
 * @param str 
 * @param endptr 
 * @param base 
 * @return long long 
 */
long long strtoll(char const* str, char** endptr, int base)
{
    char* parse_ptr = const_cast<char*>(str);
    strtons(parse_ptr, &parse_ptr);
    const Sign sign = strtosign(parse_ptr, &parse_ptr);

    if (base == 0) {
        if (*parse_ptr == '0') {
            if (tolower(*(parse_ptr + 1)) == 'x') {
                base = 16;
                parse_ptr += 2;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    }

    LongLongParser digits { sign, base };
    bool digits_usable = false;
    bool should_continue = true;
    bool overflow = false;
    do {
        bool is_a_digit;
        if (overflow) {
            is_a_digit = digits.parse_digit(*parse_ptr) >= 0;
        } else {
            DigitConsumeDecision decision = digits.consume(*parse_ptr);
            switch (decision) {
            case DigitConsumeDecision::Consumed:
                is_a_digit = true;
                digits_usable = true;
                break;
            case DigitConsumeDecision::PosOverflow:
            case DigitConsumeDecision::NegOverflow:
                is_a_digit = true;
                overflow = true;
                break;
            case DigitConsumeDecision::Invalid:
                is_a_digit = false;
                break;
            default:
                VERIFY_NOT_REACHED();
            }
        }

        should_continue = is_a_digit;
        parse_ptr += should_continue;
    } while (should_continue);

    if (!digits_usable) {
        if (endptr)
            *endptr = const_cast<char*>(str);
        return 0;
    }

    if (endptr)
        *endptr = parse_ptr;

    if (overflow) {
        errno = ERANGE;
        if (sign != Sign::Negative) {
            return LONG_LONG_MAX;
        } else {
            return LONG_LONG_MIN;
        }
    }

    return digits.number();
}

/**
 * @param str 
 * @param endptr 
 * @param base 
 * @return unsigned long long 
 */
unsigned long long strtoull(char const* str, char** endptr, int base)
{
    char* parse_ptr = const_cast<char*>(str);
    strtons(parse_ptr, &parse_ptr);

    if (base == 16) {
        if (*parse_ptr == '0') {
            if (tolower(*(parse_ptr + 1)) == 'x')
                parse_ptr += 2;
        }
    }

    if (base == 0) {
        if (*parse_ptr == '0') {
            if (tolower(*(parse_ptr + 1)) == 'x') {
                base = 16;
                parse_ptr += 2;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    }

    ULongLongParser digits { Sign::Positive, base };
    bool digits_usable = false;
    bool should_continue = true;
    bool overflow = false;
    do {
        bool is_a_digit;
        if (overflow) {
            is_a_digit = digits.parse_digit(*parse_ptr) >= 0;
        } else {
            DigitConsumeDecision decision = digits.consume(*parse_ptr);
            switch (decision) {
            case DigitConsumeDecision::Consumed:
                is_a_digit = true;
                digits_usable = true;
                break;
            case DigitConsumeDecision::PosOverflow:
            case DigitConsumeDecision::NegOverflow:
                is_a_digit = true;
                overflow = true;
                break;
            case DigitConsumeDecision::Invalid:
                is_a_digit = false;
                break;
            default:
                VERIFY_NOT_REACHED();
            }
        }

        should_continue = is_a_digit;
        parse_ptr += should_continue;
    } while (should_continue);

    if (!digits_usable) {
        if (endptr)
            *endptr = const_cast<char*>(str);
        return 0;
    }

    if (endptr)
        *endptr = parse_ptr;

    if (overflow) {
        errno = ERANGE;
        return LONG_LONG_MAX;
    }

    return digits.number();
}

uint32_t arc4random(void)
{
    uint32_t buf;
    arc4random_buf(&buf, sizeof(buf));
    return buf;
}

static pthread_mutex_t s_randomness_mutex = PTHREAD_MUTEX_INITIALIZER;
static u8* s_randomness_buffer;
static size_t s_randomness_index;

/**
 * @param buffer 
 * @param buffer_size 
 */
void arc4random_buf(void* buffer, size_t buffer_size)
{
    pthread_mutex_lock(&s_randomness_mutex);

    size_t bytes_needed = buffer_size;
    auto* ptr = static_cast<u8*>(buffer);

    while (bytes_needed > 0) {
        if (!s_randomness_buffer || s_randomness_index >= PAGE_SIZE) {
            if (!s_randomness_buffer) {
                s_randomness_buffer = static_cast<u8*>(mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_RANDOMIZED, 0, 0));
                VERIFY(s_randomness_buffer != MAP_FAILED);
                __pthread_fork_atfork_register_child(
                    [] {
                        munmap(s_randomness_buffer, PAGE_SIZE);
                        s_randomness_buffer = nullptr;
                        s_randomness_index = 0;
                    });
            }
            syscall(SC_getrandom, s_randomness_buffer, PAGE_SIZE);
            s_randomness_index = 0;
        }

        size_t available_bytes = PAGE_SIZE - s_randomness_index;
        size_t bytes_to_copy = min(bytes_needed, available_bytes);

        memcpy(ptr, s_randomness_buffer + s_randomness_index, bytes_to_copy);

        s_randomness_index += bytes_to_copy;
        bytes_needed -= bytes_to_copy;
        ptr += bytes_to_copy;
    }

    pthread_mutex_unlock(&s_randomness_mutex);
}

/**
 * @param max_bounds 
 * @return uint32_t 
 */
uint32_t arc4random_uniform(uint32_t max_bounds)
{
    return Mods::get_random_uniform(max_bounds);
}

/**
 * @param pathname 
 * @param buffer 
 * @return char* 
 */
char* realpath(char const* pathname, char* buffer)
{
    if (!pathname) {
        errno = EFAULT;
        return nullptr;
    }
    size_t size = PATH_MAX;
    bool self_allocated = false;
    if (buffer == nullptr) {
        size = 64;
        buffer = (char*)malloc(size);
        self_allocated = true;
    }
    Syscall::SC_realpath_params params { { pathname, strlen(pathname) }, { buffer, size } };
    int rc = syscall(SC_realpath, &params);
    if (rc < 0) {
        if (self_allocated)
            free(buffer);
        errno = -rc;
        return nullptr;
    }
    if (self_allocated && static_cast<size_t>(rc) > size) {
        free(buffer);
        size = static_cast<size_t>(rc);
        buffer = (char*)malloc(size);
        params.buffer = { buffer, size };
        rc = syscall(SC_realpath, &params);
        if (rc < 0) {
            free(buffer);
            errno = -rc;
            return nullptr;
        }
        size_t new_size = static_cast<size_t>(rc);
        if (new_size < size) {
            free(buffer);
            errno = EAGAIN;
            return nullptr;
        }
    }
    errno = 0;
    return buffer;
}

/**
 * @param flags 
 * @return int 
 */
int posix_openpt(int flags)
{
    if (flags & ~(O_RDWR | O_NOCTTY | O_CLOEXEC)) {
        errno = EINVAL;
        return -1;
    }

    return open("/dev/ptmx", flags);
}

/**
 * @param fd 
 * @return int 
 */
int grantpt([[maybe_unused]] int fd)
{
    return 0;
}

/**
 * @param fd 
 * @return int 
 */
int unlockpt([[maybe_unused]] int fd)
{
    return 0;
}
}

/**
 * @param status 
 */
void _Exit(int status)
{
    _exit(status);
}

/**
 * @param memptr 
 * @param alignment 
 * @param size 
 * @return int 
 */
int posix_memalign(void** memptr, size_t alignment, size_t size)
{
    (void)memptr;
    (void)alignment;
    (void)size;
    TODO();
}
