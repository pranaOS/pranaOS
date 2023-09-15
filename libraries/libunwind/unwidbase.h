/**
 * @file unwidbase.h
 * @author Krisna Pranav
 * @brief unwind base
 * @version 6.0
 * @date 2023-09-15
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <stdint.h>

extern "C" 
{

    enum _Unwind_Reason_Code 
    {
        _URC_NO_REASON = 0,                
        _URC_FOREIGN_EXCEPTION_CAUGHT = 1, 
        _URC_FATAL_PHASE2_ERROR = 2,       
        _URC_FATAL_PHASE1_ERROR = 3,       
        _URC_NORMAL_STOP = 4,              
        _URC_END_OF_STACK = 5,             
        _URC_HANDLER_FOUND = 6,            
        _URC_INSTALL_CONTEXT = 7,          
        _URC_CONTINUE_UNWIND = 8           
    }; // enum 

    struct _Unwind_Exception;

    typedef void (*_Unwind_Exception_Cleanup_Fn)(_Unwind_Reason_Code reason, struct _Unwind_Exception* exc);

    struct _Unwind_Exception 
    {
        uint64_t exception_class;                       
        _Unwind_Exception_Cleanup_Fn exception_cleanup; 
        uint32_t private_1;                             
        uint32_t private_2;                             
    }; // struct

    struct _Unwind_Context;

    /**
     * @param exception_object 
     * @return _Unwind_Reason_Code 
     */
    _Unwind_Reason_Code _Unwind_RaiseException(struct _Unwind_Exception* exception_object);

    /**
     * @param exception_object 
     */
    void _Unwind_Resume(struct _Unwind_Exception* exception_object);

    /**
     * @param exception_object 
     */
    void _Unwind_DeleteException(struct _Unwind_Exception* exception_object);

    /**
     * @param context 
     * @param index 
     * @return uint32_t 
     */
    uint32_t _Unwind_GetGR(struct _Unwind_Context* context, int index);

    /**
     * @param context 
     * @param index 
     * @param new_value 
     */
    void _Unwind_SetGR(struct _Unwind_Context* context, int index, uint32_t new_value);

    /**
     * @param context 
     * @return uint32_t 
     */
    uint32_t _Unwind_GetIP(struct _Unwind_Context* context);

    /**
     * @param context 
     * @param new_value 
     */
    void _Unwind_SetIP(struct _Unwind_Context* context, uint32_t new_value);

    /**
     * @param context 
     * @return uint32_t 
     */
    uint32_t _Unwind_GetLanguageSpecificData(struct _Unwind_Context* context);

    /**
     * @param context 
     * @return uint32_t 
     */
    uint32_t _Unwind_GetRegionStart(struct _Unwind_Context* context);

    /**
     * @param context 
     * @return uint32_t 
     */
    uint32_t _Unwind_GetCFA(struct _Unwind_Context* context);

    typedef _Unwind_Reason_Code (*__personality_routine)(int version, _Unwind_Action actions, uint64_t exceptionClass, struct _Unwind_Exception* exceptionObject, struct _Unwind_Context* context);

    typedef int _Unwind_Action;

    enum __UnwindActions 
    {
        _UA_SEARCH_PHASE = 1,  
        _UA_CLEANUP_PHASE = 2, 
        _UA_HANDLER_FRAME = 4, 
        _UA_FORCE_UNWIND = 8                              
    };

    typedef _Unwind_Reason_Code (*_Unwind_Stop_Fn)(int version, _Unwind_Action actions, uint64_t exceptionClass, struct _Unwind_Exception* exceptionObject, struct _Unwind_Context* context, void* stop_parameter);


    /**
     * @param exception_object 
     * @param stop 
     * @param stop_parameter 
     * @return _Unwind_Reason_Code 
     */
    _Unwind_Reason_Code _Unwind_ForcedUnwind(struct _Unwind_Exception* exception_object, _Unwind_Stop_Fn stop, void* stop_parameter);

} // extern "C"
