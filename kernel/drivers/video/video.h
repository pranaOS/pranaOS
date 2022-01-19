//
// Created by KrisnaPranav on 19/01/22.
//

#pragma once

#include <kernel/drivers/driver.h>
#include <kernel/internal/pci.h>
#include <kernel/internal/graphics.h>
#include <kernel/system/interruptmanager.h>

namespace Kernel::Driver {
    #define VIDEO_VENDORID 0x15AD
    #define VIDEO_DEVICEID 0x0405

    #define SVGA_INDEX_PORT         0x0
    #define SVGA_VALUE_PORT         0x1
    #define SVGA_BIOS_PORT          0x2
    #define SVGA_IRQSTATUS_PORT     0x8

    #define SVGA_MAGIC         0x900000UL
    #define SVGA_MAKE_ID(ver)  (SVGA_MAGIC << 8 | (ver))

    #define SVGA_VERSION_2     2
    #define SVGA_ID_2          SVGA_MAKE_ID(SVGA_VERSION_2)

    #define SVGA_REG_ID 0
    #define SVGA_REG_FB_START 13
    #define SVGA_REG_FB_ENABLE 1
    #define SVGA_REG_FB_WIDTH 2
    #define SVGA_REG_FB_HEIGHT 3
    #define SVGA_REG_FB_BITSPERPIXEL 7

    class video : public Driver, public graphicsDevice {
      public:
        video(pciDevice* pciDev);

        bool initialize();
      private:
        pciDevice* pcidevice;

        void writeRegister(ak::uint32_t reg, ak::uint32_t value);
        ak::uint32_t readRegister(ak::uin32_t reg);
    };
}