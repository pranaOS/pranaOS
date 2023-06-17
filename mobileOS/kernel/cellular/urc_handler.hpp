/**
 * @file urc_handler.hpp
 * @author Krisna Pranav
 * @brief URCHandler
 * @version 1.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Cellular::URC {
    class Clip;
    class Creg;
    class Cmti;
    class Cusd;
    class Ctze;
    class Qind;
    class Cpin;
    class Qiurc;
    class Ring;
    class PowerdDown;

    class URCHandler {
    public:
        virtual void Handle(Clip &urc) =  0;
    };
}