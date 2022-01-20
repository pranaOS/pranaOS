#pragma once

#include <kernel/drivers/driver.h>
#include <kernel/usb/usbdevice.h>

namespace Kernel {
    typedef struct interruptTransfer interruptTransfer_t;

    class usbDriver : public Kernel::driver {
      public:
        usbDevice* device;
      public:
        usbDriver(usbDevice* dev, char* driverName);
        virtual ~usbDriver();

        virtual void initialize();
        virtual void deInitialize();

        virtual bool handleInterruptPacket(interruptTransfer_t* transfer);
    };
}