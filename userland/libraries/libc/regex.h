/**
 * @file regex.h
 * @author Krisna Pranav
 * @brief regex
 * @version 0.1
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <stddef.h>
#include <sys/types.h>

__BEGIN_DECLS

typedef ssize_t regoff_t;

typedef struct {
    void* __data;
    size_t re_nsub;
} regex_t;

enum __Regex_Error {
    __Regex_NoError,
    __Regex_InvalidPattern,             
    __Regex_InvalidCollationElement,    
    __Regex_InvalidCharacterClass,      
    __Regex_InvalidTrailingEscape,      
    __Regex_InvalidNumber,              
    __Regex_MismatchingBracket,         
    __Regex_MismatchingParen,           
    __Regex_MismatchingBrace,           
    __Regex_InvalidBraceContent,        
    __Regex_InvalidBracketContent,      
    __Regex_InvalidRange,               
    __Regex_InvalidRepetitionMarker,    
    __Regex_ReachedMaxRecursion,        
    __Regex_EmptySubExpression,         
    __Regex_InvalidCaptureGroup,        
    __Regex_InvalidNameForCaptureGroup, 
    __Regex_InvalidNameForProperty,     
    __Regex_DuplicateNamedCapture,      
}; // enum __Regex_Error

enum ReError {
    REG_NOERR = __Regex_NoError,
    REG_BADPAT = __Regex_InvalidPattern,            
    REG_ECOLLATE = __Regex_InvalidCollationElement, 
    REG_ECTYPE = __Regex_InvalidCharacterClass,     
    REG_EESCAPE = __Regex_InvalidTrailingEscape,    
    REG_ESUBREG = __Regex_InvalidNumber,            
    REG_EBRACK = __Regex_MismatchingBracket,        
    REG_EPAREN = __Regex_MismatchingParen,          
    REG_EBRACE = __Regex_MismatchingBrace,          
    REG_BADBR = __Regex_InvalidBraceContent,        
    REG_ERANGE = __Regex_InvalidRange,              
    REG_BADRPT = __Regex_InvalidRepetitionMarker,  
    REG_EMPTY_EXPR = __Regex_EmptySubExpression,    
    REG_ENOSYS,                                     
    REG_ESPACE,                                     
    REG_NOMATCH,                                    
}; // enum ReError

typedef struct {
    regoff_t rm_so;  
    regoff_t rm_eo;  
    regoff_t rm_cnt; 
} regmatch_t;

enum __RegexAllFlags {
    __Regex_Global = 1,                                      
    __Regex_Insensitive = __Regex_Global << 1,               
    __Regex_Ungreedy = __Regex_Global << 2,                  
    __Regex_Unicode = __Regex_Global << 3,                   
    __Regex_Extended = __Regex_Global << 4,                  
    __Regex_Extra = __Regex_Global << 5,                     
    __Regex_MatchNotBeginOfLine = __Regex_Global << 6,       
    __Regex_MatchNotEndOfLine = __Regex_Global << 7,         
    __Regex_SkipSubExprResults = __Regex_Global << 8,        
    __Regex_StringCopyMatches = __Regex_Global << 9,         
    __Regex_SingleLine = __Regex_Global << 10,               
    __Regex_Sticky = __Regex_Global << 11,                   
    __Regex_Multiline = __Regex_Global << 12,                
    __Regex_SkipTrimEmptyMatches = __Regex_Global << 13,     
    __Regex_SingleMatch = __Regex_Global << 14,              
    __Regex_Internal_Stateful = __Regex_Global << 15,        
    __Regex_Internal_BrowserExtended = __Regex_Global << 16, 
    __Regex_Internal_ConsiderNewline = __Regex_Global << 17, 
    __Regex_Last = __Regex_SingleMatch
}; // enum __RegexAllFlags

#define REG_EXTENDED __Regex_Extended                
#define REG_ICASE __Regex_Insensitive                
#define REG_NOSUB __Regex_SkipSubExprResults         
#define REG_GLOBAL __Regex_Global                    
#define REG_NEWLINE (__Regex_Multiline | REG_GLOBAL) 

#define REG_NOTBOL __Regex_MatchNotBeginOfLine 
#define REG_NOTEOL __Regex_MatchNotEndOfLine   

#define REG_SEARCH __Regex_Last << 1

/**
 * @return int 
 */
int regcomp(regex_t*, char const*, int);

/**
 * @return int 
 */
int regexec(regex_t const*, char const*, size_t, regmatch_t[], int);

/**
 * @return size_t 
 */
size_t regerror(int, regex_t const*, char*, size_t);

void regfree(regex_t*);

__END_DECLS
