#pragma once

#include "systemcomponent.h"

namespace Kernel {
    namespace system {
        class graphicsDevice {
        public:
            ak::uint32_t width;
            ak::uint32_t height;
            ak::uint8_t bpp;
            ak::uint32_t framebufferPhys;
            char* identifer = 0;

            graphicsDevice(char* name);
            virtual ~graphicsDevice();
            virtual bool selectBestVideoMode();

            static graphicsDevice* getBestDevice();
        }
    }
}