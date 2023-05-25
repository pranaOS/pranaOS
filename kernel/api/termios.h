/**
 * @file termios.h
 * @author Krisna Pranav
 * @brief TERMIOS
 * @version 0.1
 * @date 2023-05-25
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "cdefs.h"
#include "stdint.h"

__DECL_BEGIN

#define NCCS 32

typedef uint32_t speed_t;
typedef uint32_t tcflag_t;
typedef uint8_t cc_t;

struct termios {
	tcflag_t c_iflag;
	tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t c_cc[NCCS];
	tcflag_t c_ispeed;
	tcflag_t c_ospeed;
};

struct winsize {
    unsigned short ws_row;
    unsigned short ws_col;
    unsigned short ws_xpixel;
    unsigned short ws_ypixel;
};

#define IGNBRK	0x1
#define BRKINT	0x2
#define IGNPAR	0x4
#define PARMRK	0x8
#define INPCK	0x10
#define ISTRIP	0x20
#define INLCR	0x40
#define IGNCR	0x80
#define ICRNL	0x100
#define IUCLC	0x200
#define IXON	0x400
#define IXANY	0x800
#define IXOFF	0x1000
#define IMAXBEL	0x2000
#define IUTF8	0x4000

#define OPOST	0x1
#define OLCUC	0x2
#define ONLCR	0x4
#define OCRNL	0x8
#define ONOCR	0x10
#define ONLRET	0x20
#define OFILL	0x40
#define OFDEL	0x80
#define NLDLY	0x100
#define CRDLY	0x200
#define TABDLY	0x400
#define BSDLY	0x800
#define VTDLY	0x1000
#define FFDLY	0x2000

#define CSIZE	0x6
#define CS5		0x0
#define CS6		0x2
#define CS7		0x4
#define CS8		0x6
#define CSTOPB	0x8
#define CREAD	0x10
#define PARENB	0x20
#define PARODD	0x40
#define HUPCL	0x80
#define CLOCAL	0x100

#define ISIG	0x1
#define ICANON	0x2
#define XCASE	0x4
#define ECHO	0x8
#define ECHOE	0x10
#define EHCOK	0x20
#define ECHONL	0x40
#define ECHOCTL	0x80
#define ECHOPRT	0x100
#define ECHOKE	0x200
#define DEFECHO	0x400
#define FLUSHO	0x800
#define NOFLSH	0x1000
#define TOSTOP	0x2000
#define PENDIN	0x4000
#define IEXTEN	0x8000

#define VDISCARD	0
#define VDSUSP		1
#define VEOF		2
#define VEOL		3
#define VEOL2		4
#define VERASE		5
#define VINTR		6
#define VKILL		7
#define VLNEXT		8
#define VMIN		9
#define VQUIT		10
#define VREPRINT	11
#define VSTART		12
#define VSTATUS		13
#define VSTOP		14
#define VSUSP		15
#define VSWTCH		16
#define VTIME		17
#define VWERASE		18

#define TCSANOW		1
#define TCSADRAIN	2
#define TCSAFLUSH	3

#define TCOOFF	0
#define TCOON	1
#define TCIOFF	2
#define TCION	3

#define TCIFLUSH	0
#define TCOFLUSH	1
#define TCIOFLUSH	2

__DECL_END