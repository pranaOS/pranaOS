//
// Created by KrisnaPranav on 19/01/22.
//

#pragma once

#include <drivers/driver.h>
#include <internal/pci.h>
#include <system/interrupthandler.h>
#include <internal/graphics.h>

namespace Kernel {

  #define VMWARESVGAII_VENDORID 0x15AD
  #define VMWARESVGAII_DEVICEID 0x0405

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
  #define SVGA_REG_FB_Enable 1
  #define SVGA_REG_FB_Width 2
  #define SVGA_REG_FB_Height 3
  #define SVGA_REG_FB_BitsPerPixel 7

  class video : public driver, public graphicsDevice {
    public:
        video(PCIDevice* pciDev);
        bool initialize();
        bool selectBestVideoMode();

    private:
        pciDevice* pcidevice;
        void writeRegister(ak::uint32_t reg, ak::uint32_t value);
        ak::uint32_t readRegister(ak::uint32_t reg);
  };
}
