#pragma once

#include "systemcomponent.h"

namespace Kernel {
    class graphicsDevice {
    public:
        uint32_t width;
        uint32_t height;
        uint8_t bpp;
        uint32_t framebufferPhys;
        char* identifier = 0;

        graphicsDevice(char* name);
        virtual ~graphicsDevice();
        virtual bool selectBestVideoMode();
            
        uint32_t getBufferSize();
            
        static graphicsDevice* getBestDevice();
    
    private:
        graphicsDevice();
        static graphicsDevice* device();
    };
}