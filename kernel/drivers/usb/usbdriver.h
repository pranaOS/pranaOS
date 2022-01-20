#pragma once

#include <kernel/drivers/driver.h>

namespace Kernel {
    
    typedef struct interruptTransfer {
        uint8_t* bufferPointer;
        uint32_t bufferPhys;
        int bufferLen;

        void* td;
        int numTd;
        void* qh;
        int endPoint;
    } interruptTransfer_t;

    class usbDriver : public Kernel::driver {
    public:
        virtual ~usbDriver();

        virtual void initialize();
        void deInitialize();

        virtual bool handleInterruptPacket(interruptTransfer_t* transfer);
    }
}