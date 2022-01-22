//
// Created by KrisnaPranav on 20/01/22.
//

#pragma once

#include <kernel/drivers/driver.h>
#include <kernel/components/pci.h>
#include <system/interrupthandler.h>
#include "usbdevice.h"

namespace Kernel {
    enum usbControllerType {
        UHCI,
        OHCI,
        EHCI,
        XHCI
    };

    class USBDriver;
    typedef struct interruptTransfer {
        uint8_t* bufferPointer;
        uint32_t bufferPhys;
        int bufferLen;

        usbDriver* handler;
        int queueIndex;

        uint32_t tdPhys;
        void* td;
        int numTd;

        void* qh;
        int endpoint;
    } interruptTransfer_t;

    class usbDevice;

    class usbController {
    public:
        usbControllerType type;
        usbController(usbControllerType usbType);

        virtual void setup();

        virtual void controllerChecksThread();

        virtual bool bulkIn(usbDevice* device, void* retBuffer, int len, int endP);

        virtual bool bulkOut(usbDevice* device, void* sendBuffer, int len, int endP);

        virtual bool controlIn(usbDevice* device, void* target = 0, const int len = 0, const uint8_t requestType = 0, const uint8_t request = 0, const uint16_t valueHigh = 0, const uint16_t valueLow = 0, const uint16_t index = 0);
        virtual bool controlOut(usbDevice* device, const int len = 0, const uint8_t requestType = 0, const uint8_t request = 0, const uint16_t valueHigh = 0, const uint16_t valueLow = 0, const uint16_t index = 0);
            
        virtual void interruptIn(usbDevice* device, int len, int endP);

        bool getDeviceDescriptor(struct DEVICE_DESC* dev_desc, usbDevice* device);

        bool getStringDescriptor(struct STRING_DESC* stringDesc, usbDevice* device, uint16_t index, uint16_t lang = 0);
            
        uint8_t* getConfigDescriptor(usbDevice* device);
            
        bool setConfiguration(usbDevice* device, uint8_t config);

        int getConfiguration(usbDevice* device);
            
        int getMaxLuns(usbDevice* device);

    protected:
        List<interruptTransfer_t*> interrupTransfers;
    };
}