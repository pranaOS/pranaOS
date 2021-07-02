#pragma once

// includes
#include <Kernel/Devices/CharacterDevice.h>
#include <Kernel/Random.h>

namespace Kernel {

class HIDDevice : public CharacterDevice {
public:
    enum class Type {
        Unknown = 0,
        Keyboard,
        Mouse,
    };

    virtual Type instrument_type() const = 0;
    virtual void enable_interrupts() = 0;

protected:
    HIDDevice(unsigned major, unsigned minor)
        : CharacterDevice(major, minor)
    {
    }

    EntropySource m_entropy_source;
};

}