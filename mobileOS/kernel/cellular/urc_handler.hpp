/**
 * @file urc_handler.hpp
 * @author Krisna Pranav
 * @brief URCHandler
 * @version 6.0
 * @date 2023-06-17
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

namespace Cellular::URC
{

/**
 * @brief handling classes
 *
 */
class Clip;
class Creg;
class Cmti;
class Cusd;
class Ctze;
class Qind;
class Cpin;
class Qiurc;
class Ring;
class PoweredDown;
class URCResponse;
class QSimstat;

/**
 * @brief URCHandler
 *
 */
class URCHandler
{
  public:
    virtual void Handle(Clip& urc) = 0;
    virtual void Handle(Creg& urc) = 0;
    virtual void Handle(Cmti& urc) = 0;
    virtual void Handle(Cusd& urc) = 0;
    virtual void Handle(Ctze& urc) = 0;
    virtual void Handle(Qind& urc) = 0;
    virtual void Handle(Cpin& urc) = 0;
    virtual void Handle(Qiurc& urc) = 0;
    virtual void Handle(Ring& urc) = 0;
    virtual void Handle(PoweredDown& urc) = 0;
    virtual void Handle(URCResponse& urc) = 0;
    virtual void Handle(QSimstat& urc) = 0;
};
} // namespace Cellular::URC