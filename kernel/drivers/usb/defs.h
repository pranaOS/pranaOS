#ifndef KERNEL_DRIVER_USB
#define KERNEL_DRIVER_USB

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
        ak::uint8_t  len;
        ak::uint8_t  type;
        ak::uint16_t usb_ver;
        ak::uint8_t  _class;
        ak::uint8_t  subclass;
        ak::uint8_t  protocol;
        ak::uint8_t  max_packet_size;
        ak::uint16_t vendorid;
        ak::uint16_t productid;
        ak::uint16_t device_rel;
        ak::uint8_t  manuf_indx;   
        ak::uint8_t  prod_indx;    
        ak::uint8_t  serial_indx;  
        ak::uint8_t  configs;      
    } __attribute__((packed));

    struct REQUEST_PACKET {
        ak::uint8_t  request_type;
        ak::uint8_t  request;
        ak::uint16_t value;
        ak::uint16_t index;
        ak::uint16_t length;
    } __attribute__((packed));

    struct CONFIG_DESC {
        ak::uint8_t  len;
        ak::uint8_t  type;
        ak::uint16_t tot_len;
        ak::uint8_t  num_interfaces;
        ak::uint8_t  config_val;
        ak::uint8_t  config_indx;
        ak::uint8_t  bm_attrbs;
        ak::uint8_t  max_power;
    } __attribute__((packed));

    struct STRING_DESC {
        ak::uint8_t  len;         
        ak::uint8_t  type;
        ak::uint16_t string[127];
    } __attribute__((packed));

    struct INTERFACE_ASSOSIATION_DESC {
        ak::uint8_t  len;             
        ak::uint8_t  type;            
        ak::uint8_t  interface_num;   
        ak::uint8_t  count;           
        ak::uint8_t  _class;          
        ak::uint8_t  subclass;        
        ak::uint8_t  protocol;        
        ak::uint8_t  function_indx;   
    } __attribute__((packed));

    struct INTERFACE_DESC {
            ak::uint8_t  len;
            ak::uint8_t  type;
            ak::uint8_t  interface_num;
            ak::uint8_t  alt_setting;
            ak::uint8_t  num_endpoints;
            ak::uint8_t  interface_class;
            ak::uint8_t  interface_sub_class;
            ak::uint8_t  interface_protocol;
            ak::uint8_t  interface_indx;
    } __attribute__((packed));

    struct ENDPOINT_DESC {
        ak::uint8_t  len;
        ak::uint8_t  type;
        ak::uint8_t  end_point;        
        ak::uint8_t  bm_attrbs;        
        ak::uint16_t max_packet_size;  
        ak::uint8_t  interval;
    } __attribute__((packed));

    struct IF_HID_DESC {
        ak::uint8_t  len;
        ak::uint8_t  type;
        ak::uint16_t release;
        ak::uint8_t  countryCode;
        ak::uint8_t  numDescriptors;
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


#endif