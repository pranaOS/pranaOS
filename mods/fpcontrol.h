/**
 * @file fpcontrol.h
 * @author Krisna Pranav
 * @brief fp contorl
 * @version 6.0
 * @date 2025-04-22
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

VALIDATE_IS_X86();

namespace Mods 
{

    enum class RoundingMode : u16 
    {
        NEAREST = 0b00,
        DOWN = 0b01,
        UP = 0b10,
        TRUNC = 0b11
    }; // enum class RoundingMode : u16 

    union X87ControlWord 
    {
        u16 cw;
        struct {
            u16 mask_invalid : 1;              
            u16 mask_denorm : 1;               
            u16 mask_zero_div : 1;             
            u16 mask_overflow : 1;             
            u16 mask_underflow : 1;            
            u16 mask_precision : 1;            
            u16 : 2;                           
            u16 precision : 2;                 
            RoundingMode rounding_control : 2; 
            u16 infinity_control : 1;          
            u16 : 3;                           
        };
    }; // union X87ControlWord 

    static_assert(sizeof(X87ControlWord) == sizeof(u16));

    union MXCSR 
    {
        u32 mxcsr;
        struct 
        {
            u32 invalid_operation_flag : 1;    
            u32 denormal_operation_flag : 1;   
            u32 divide_by_zero_flag : 1;       
            u32 overflow_flag : 1;             
            u32 underflow_flag : 1;            
            u32 precision_flag : 1;            
            u32 denormals_are_zero : 1;        
            u32 invalid_operation_mask : 1;    
            u32 denormal_operation_mask : 1;   
            u32 divide_by_zero_mask : 1;       
            u32 overflow_mask : 1;             
            u32 underflow_mask : 1;            
            u32 precision_mask : 1;            
            RoundingMode rounding_control : 2; 
            u32 flush_to_zero : 1;             
            u32 __reserved : 16;
        };
    }; // union MXCSR 

    static_assert(sizeof(MXCSR) == sizeof(u32));

    /**
     * @brief Get the cw x87 object
     * 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE X87ControlWord get_cw_x87()
    {
        X87ControlWord control_word;
        asm("fnstcw %0"
            : "=m"(control_word));
        return control_word;
    }

    /**
     * @brief Set the cw x87 object
     * 
     * @param control_word 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE void set_cw_x87(X87ControlWord control_word)
    {
        asm("fldcw %0" ::"m"(control_word));
    }

    /**
     * @brief Get the mxcsr object
     * 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE MXCSR get_mxcsr()
    {
        MXCSR mxcsr;
        asm("stmxcsr %0"
            : "=m"(mxcsr));
        return mxcsr;
    }

    /**
     * @brief Set the mxcsr object
     * 
     * @param mxcsr 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE void set_mxcsr(MXCSR mxcsr)
    {
        asm("ldmxcsr %0" ::"m"(mxcsr));
    }

    class X87RoundingModeScope 
    {
    public:
        /**
         * @brief Construct a new X87RoundingModeScope object
         * 
         * @param rounding_mode 
         */
        X87RoundingModeScope(RoundingMode rounding_mode)
        {
            m_cw = get_cw_x87();
            auto cw = m_cw;
            cw.rounding_control = rounding_mode;
            set_cw_x87(cw);
        }

        /**
         * @brief Destroy the X87RoundingModeScope object
         * 
         */
        ~X87RoundingModeScope()
        {
            set_cw_x87(m_cw);
        }

    private:
        X87ControlWord m_cw; 
    }; // class X87RoundingModeScope 

    class SSERoundingModeScope 
    {
    public:
        /**
         * @brief Construct a new SSERoundingModeScope object
         * 
         * @param rounding_mode 
         */
        SSERoundingModeScope(RoundingMode rounding_mode)
        {
            m_mxcsr = get_mxcsr();
            auto mxcsr = m_mxcsr;
            mxcsr.rounding_control = rounding_mode;
            set_mxcsr(mxcsr);
        }

        /**
         * @brief Destroy the SSERoundingModeScope object
         * 
         */
        ~SSERoundingModeScope()
        {
            set_mxcsr(m_mxcsr);
        }

    private:
        MXCSR m_mxcsr;
    }; // class SSERoundingModeScope 

} // namespace Mods
