#pragma once

#include <kernel/drivers/driver.h>
#include <system/interrupthandler.h>

namespace Kernel {
    namespace drivers {
        struct mousePacket {

            unsigned char leftBTN : 1;
            unsigned char rightBTN : 1;
            unsigned char middleBTN : 1;
            unsigned char always1 : 1;
            unsigned char xSign : 1;
            unsigned char ySign : 1;
            unsigned char xOverflow : 1;
            unsigned char yOverflow : 1;
            signed char xMovement;
            signed char yMovement;
            signed char zMovement; 
        };
            

        class ps2MouseDriver : public interruptHandler, public driver {
        public:
            ps2MouseDriver();

            bool initialize();
            ak::uint32_t handleInterrupt(ak::uint32_t esp);

            bool enableScrollWheel();
            bool setSampleRate(ak::uint8_t value);
            void processPacket();

        private:
            #define MOUSE_DATA 0x60
            #define MOUSE_COMMAND 0x64
            #define MOUSE_ACK 0xFA

            ak::uint8_t mouseID = 0;
            ak::uint8_t mouseCycle = 0;
            signed char* packetBuffer;

            bool ready = false;
        };
    }
}
