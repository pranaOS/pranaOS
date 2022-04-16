#pragma once

#include <ak/types.h>

namespace Kernel {

    #define USB_TDRSTR      50   
    #define USB_TDRST       10   
    #define USB_TRHRSI      3   
    #define USB_TRSTRCY     10   
    #define PINDC_OFF       0b00
    #define PINDC_AMBER     0b01
    #define PINDC_GREEN     0b10
    #define ENDP_CONTROL    0
    #define HID_REQUEST_SET_IDLE 0x0A
    #define HID_REQUEST_SET_PROTOCOL 0x0B

    struct DEVICE_DESC {
        uint8_t  len;
        uint8_t  type;
        uint16_t usb_ver;
        uint8_t  _class;
        uint8_t  subclass;
        uint8_t  protocol;
        uint8_t  max_packet_size;
        uint16_t vendorid;
        uint16_t productid;
        uint16_t device_rel;
        uint8_t  manuf_indx;   
        uint8_t  prod_indx;    
        uint8_t  serial_indx;  
        uint8_t  configs;      
    } __attribute__((packed));

    struct REQUEST_PACKET {
        uint8_t  request_type;
        uint8_t  request;
        uint16_t value;
        uint16_t index;
        uint16_t length;
    } __attribute__((packed));

    struct CONFIG_DESC {
        uint8_t  len;
        uint8_t  type;
        uint16_t tot_len;
        uint8_t  num_interfaces;
        uint8_t  config_val;
        uint8_t  config_indx;
        uint8_t  bm_attrbs;
        uint8_t  max_power;
    } __attribute__((packed));

    struct STRING_DESC {
        uint8_t  len;         
        uint8_t  type;
        uint16_t string[127];
    } __attribute__((packed));

    struct INTERFACE_ASSOSIATION_DESC {
        uint8_t  len;             
        uint8_t  type;            
        uint8_t  interface_num;   
        uint8_t  count;           
        uint8_t  _class;          
        uint8_t  subclass;        
        uint8_t  protocol;        
        uint8_t  function_indx;   
    } __attribute__((packed));

    struct INTERFACE_DESC {
            uint8_t  len;
            uint8_t  type;
            uint8_t  interface_num;
            uint8_t  alt_setting;
            uint8_t  num_endpoints;
            uint8_t  interface_class;
            uint8_t  interface_sub_class;
            uint8_t  interface_protocol;
            uint8_t  interface_indx;
    } __attribute__((packed));

    struct ENDPOINT_DESC {
        uint8_t  len;
        uint8_t  type;
        uint8_t  end_point;        
        uint8_t  bm_attrbs;        
        uint16_t max_packet_size;  
        uint8_t  interval;
    } __attribute__((packed));

    struct IF_HID_DESC {
        uint8_t  len;
        uint8_t  type;
        uint16_t release;
        uint8_t  countryCode;
        uint8_t  numDescriptors;
    } __attribute__((packed));

    #define DEV_TO_HOST     0x80
    #define HOST_TO_DEV     0x00
    #define REQ_TYPE_STNDRD 0x00
    #define REQ_TYPE_CLASS  0x20
    #define REQ_TYPE_VENDOR 0x40
    #define REQ_TYPE_RESV   0x60
    #define RECPT_DEVICE    0x00
    #define RECPT_INTERFACE 0x01
    #define RECPT_ENDPOINT  0x02
    #define RECPT_OTHER     0x03
    #define STDRD_GET_REQUEST   (DEV_TO_HOST | REQ_TYPE_STNDRD | RECPT_DEVICE)
    #define STDRD_SET_REQUEST   (HOST_TO_DEV | REQ_TYPE_STNDRD | RECPT_DEVICE)
    #define STDRD_SET_INTERFACE (HOST_TO_DEV | REQ_TYPE_STNDRD | RECPT_INTERFACE)

    enum deviceRequest { 
        GET_STATUS=0, 
        CLEAR_FEATURE, 
        SET_FEATURE=3, 
        SET_ADDRESS=5, 
        GET_DESCRIPTOR=6, 
        SET_DESCRIPTOR,
        GET_CONFIGURATION, 
        SET_CONFIGURATION,
        GET_INTERFACE, 
        SET_INTERFACE,
        SYNCH_FRAME,
        GET_MAX_LUNS = 0xFE, 
        BULK_ONLY_RESET
    };

    enum descriptorTypes { 
        DEVICE=1, 
        CONFIG, 
        STRING, 
        INTERFACE, 
        ENDPOINT, 
        DEVICE_QUALIFIER,
        OTHER_SPEED_CONFIG, 
        INTERFACE_POWER, 
        OTG, 
        DEBUG, 
        INTERFACE_ASSOSIATION,
        HID=0x21,
        HID_REPORT, 
        HID_PHYSICAL, 
            
        INTERFACE_FUNCTION = 0x24,
        ENDPOINT_FUNCTION,
            
        HUB = 0x29
    };

}
