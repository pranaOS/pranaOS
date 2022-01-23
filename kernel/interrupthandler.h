//
//  interrupthandler.h
//  pranaOS
//
//  Created by Krisna Pranav on 11/01/22.
//

#pragma once

#include <ak/list.h>
#include <ak/types.h>

namespace Kernel {
    class interruptHandler {
    public:
        interruptHandler(ak::uint8_t intNumber);
        virtual ak::uint32_t handleInterrupt(ak::uint32_t esp);
    };

    class interruptManager {
    public:
        static void initialize();
        static ak::uint32_t handleInterrupt(ak::uint8_t interrupt, ak::uint32_t esp);

        static void addHandler(interruptHandler* handler, ak::uint8_t interrupt);
        static void removeHandler(interruptHandler* handler, ak::uint8_t interrupt);
    
    private:
        static List<interruptHandler*>* interruptCallbacks[256];
    };
}
