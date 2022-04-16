#include <kernel/kernel.h>
#include <kernel/log.h>
#include <kernel/virtual86/vmargs.h>
#include "apm.h"

using namespace Kernel;

APMController::APMController() : systemComponent("APM Controller", "Contrls Advanced Power Management") {
    sendLog(Info, "Intializing APM Controller");
    this->Enabled = false;
    VM86Args args;
    memset(&args, 0, sizeof(VM86Args));
    args.AX = (uint16_t)0x5300 | (uint16_t)APM_FUNC_INSTALL_CHECK;

}