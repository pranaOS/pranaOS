/**
 * @file commands.hpp
 * @author Krisna Pranav
 * @brief Commands
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <chrono>
#include <vector>
#include "constants.hpp"

namespace Cellular {
    
    /**
     * @brief Cellular Activities
     * 
     */
    enum class cellularAT {
        AT,
        ECHO_OFF,
        FACTORY_RESET,
        SW_INFO,
        FLOW_CTRL_ON,
        FLOW_CTRL_OFF,
        URC_NOTIF_CHANNEL,          
        RI_PIN_AUTO_CALL,          
        RI_PIN_OFF_CALL,           
        RI_PIN_PULSE_SMS,          
        RI_PIN_OFF_SMS,            
        RI_PIN_PULSE_OTHER,        
        URC_DELAY_ON,              
        URC_UART1,                 
        AT_PIN_READY_LOGIC,        
        CSQ_URC_ON,                 
        CRC_ON,                     
        CALLER_NUMBER_PRESENTATION, 
        SMS_TEXT_FORMAT,            
        SMS_UCSC2,                  
        SMS_GSM,                    
        QSCLK_ON,                   
        QDAI,                       
        QDAI_INIT, 
        SET_URC_CHANNEL,
        CSQ,  
        CLCC, 
        CMGD, 
        CNUM, 
        CIMI, 
        QCMGR,
        ATH,       
        CHUP,      
        QHUP_BUSY, 
        ATA,       
        ATD,       
        IPR,       
        CMUX,      
        CFUN,      
        CMGS,      
        QCMGS,
        CREG,       
        QNWINFO,    
        COPS,       
        SIM_DET,    
        QSIMSTAT,   
        SIM_DET_ON, 
        SIMSTAT_ON, 
        SET_DEFAULT_SCANMODE,
        SET_SCANMODE,
        GET_SCANMODE,
        SET_DEFAULT_SERVICEDOMAIN,
        SET_SERVICEDOMAIN,
        GET_SERVICEDOMAIN,
        QGMR,               
        STORE_SETTINGS_ATW, 
        CEER,               
        QIGETERROR,         
        VTS,                
        QLDTMF,             
        CUSD_OPEN_SESSION,
        CUSD_CLOSE_SESSION,
        CUSD_SEND,
        SET_SMS_STORAGE,
        CPIN,
        GET_CPIN,
        QPINC, 
        CLCK,
        CPWD,
        ENABLE_TIME_ZONE_UPDATE,
        SET_TIME_ZONE_REPORTING,
        DISABLE_TIME_ZONE_UPDATE,
        DISABLE_TIME_ZONE_REPORTING,
        ENABLE_NETWORK_REGISTRATION_URC,
        SET_SMS_TEXT_MODE_UCS2,
        CFUN_RESET,
        CFUN_MIN_FUNCTIONALITY,   
        CFUN_FULL_FUNCTIONALITY,  
        CFUN_DISABLE_TRANSMITTING,
        LIST_MESSAGES,            
        GET_IMEI,
        CCFC, 
        CCWA, 
        CCWA_GET,
        CHLD, 
        CLIP, 
        CLIP_GET,
        CLIR, 
        CLIR_GET,
        CLIR_ENABLE,
        CLIR_DISABLE,
        CLIR_RESET,
        COLP,
        COLP_GET,
        COLP_ENABLE,
        COLP_DISABLE,
        CSSN,     
        QICSGP,   
        QIACT,    
        QIDEACT,  
        CSCA,     
        QRXGAIN,  
        CLVL,     
        QMIC,     
        QNVFR,    
        QNVFW,    
        QMBNCFG,  
        QCFG_IMS, 
        QEEC,     
        RING_URC_OFF,
        RING_URC_ON,
        CSQ_URC_OFF,
        SMS_URC_OFF,
        SMS_URC_ON,
        ACT_URC_OFF,
        ACT_URC_ON,
        SET_RNDIS,
    };

    enum class commadsSet {
        modemInit,
        simInit,
        smsInit
    };

    std::vector<cellularAT> getCommadsSet(commadsSet set);

    constexpr auto atCmdMaxRetries   = 3;
    constexpr auto atCmdRetryDelayMS = 50;
}; 