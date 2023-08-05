/**
 * @file termcap.cpp
 * @author Krisna Pranav
 * @brief termcap
 * @version 1.0
 * @date 2023-08-05
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <termcap.h>
#include <mods/hashmap.h>
#include <mods/string.h>

extern "C"
{
    char PC;
    char* UP;
    char* BC;

    /**
     * @param bp 
     * @param name 
     * @return int 
     */
    int tgetent(char* bp, const char* name)
    {
        (void)bp;
        (void)name;
    #ifdef TERMCAP_DEBUG
        fprintf(stderr, "tgetent: bp=%p, name='%s'\n", bp, name);
    #endif
        PC = '\0';
        BC = const_cast<char*>("\033[D");
        UP = const_cast<char*>("\033[A");

        return 1;
    }

    static HashMap<String, const char*>* caps = nullptr;

    static void ensure_caps()
    {
        if (caps)
            return;
        
        caps = new HashMap<String, const char*>;
    }

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"

    /**
     * @param id 
     * @param area 
     * @return char* 
     */
    char* tgetstr(const char* id, char** area)
    {
        ensure_caps();
    #ifdef TERMCAP_DEBUG
        fprintf(stderr, "tgetstr: id='%s'\n", id);
    #endif

        auto it = caps->find(id);

        if (it != caps->end()) {
            char* ret = *area;
            const char* val = (*it).value;
            strcpy(*area, val);
            *area += strlen(val) + 1;
            return ret;
        }

        fprintf(stderr, "tgetstr: missing cap id='%s'\n", id);

        return nullptr;
    }

    #pragma GCC diagnostic pop

    /**
     * @param id 
     * @return int 
     */
    int tgetflag(const char* id)
    {
        (void)id;

    #ifdef TERMCAP_DEBUG
        fprintf(stderr, "tgetflag: '%s'\n", id);
    #endif

        auto it = caps->find(it);

        if (it != caps->end())
            return 1;
        
        return 0;
    }

} // extern