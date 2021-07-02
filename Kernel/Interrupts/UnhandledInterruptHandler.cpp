// includes
#include <Kernel/Interrupts/UnhandledInterruptHandler.h>
#include <Kernel/Panic.h>

namespace Kernel {
UnhandledInterruptHandler::UnhandledInterruptHandler(u8 interrupt_vector)
    : GenericInterruptHandler(interrupt_vector)
{
}

void UnhandledInterruptHandler::handle_interrupt(const RegisterState&)
{
    PANIC("Interrupt: Unhandled vector {} was invoked for handle_interrupt(RegisterState&).", interrupt_number());
}

[[noreturn]] bool UnhandledInterruptHandler::eoi()
{
    PANIC("Interrupt: Unhandled vector {} was invoked for eoi().", interrupt_number());
}

UnhandledInterruptHandler::~UnhandledInterruptHandler()
{
}
}
