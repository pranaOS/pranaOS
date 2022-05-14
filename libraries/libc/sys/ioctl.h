/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#define _IOC_NRBITS 8
#define _IOC_TYPEBITS 8
#define _IOC_SIZEBITS 14
#define _IOC_DIRBITS 2

#define _IOC_NRMASK ((1 << _IOC_NRBITS) - 1)
#define _IOC_TYPEMASK ((1 << _IOC_TYPEBITS) - 1)
#define _IOC_SIZEMASK ((1 << _IOC_SIZEBITS) - 1)
#define _IOC_DIRMASK ((1 << _IOC_DIRBITS) - 1)

#define _IOC_NRSHIFT 0
#define _IOC_TYPESHIFT (_IOC_NRSHIFT + _IOC_NRBITS)
#define _IOC_SIZESHIFT (_IOC_TYPESHIFT + _IOC_TYPEBITS)
#define _IOC_DIRSHIFT (_IOC_SIZESHIFT + _IOC_SIZEBITS)

#define _IOC_NONE 0U
#define _IOC_WRITE 1U
#define _IOC_READ 2U

#define _IOC(dir, type, nr, size) \
	(((dir) << _IOC_DIRSHIFT) |   \
	 ((type) << _IOC_TYPESHIFT) | \
	 ((nr) << _IOC_NRSHIFT) |     \
	 ((size) << _IOC_SIZESHIFT))

/**
 * @brief invalid size argument
 * 
 */
extern unsigned int __invalid_size_argument_for_IOC;

#define _IOC_TYPECHECK(t)               \
	((sizeof(t) == sizeof(t[1]) &&      \
	  sizeof(t) < (1 << _IOC_SIZEBITS)) \
		 ? sizeof(t)                    \
		 : __invalid_size_argument_for_IOC)

#define _IO(type, nr) _IOC(_IOC_NONE, (type), (nr), 0)
#define _IOR(type, nr, size) _IOC(_IOC_READ, (type), (nr), (_IOC_TYPECHECK(size)))
#define _IOW(type, nr, size) _IOC(_IOC_WRITE, (type), (nr), (_IOC_TYPECHECK(size)))
#define _IOWR(type, nr, size) _IOC(_IOC_READ | _IOC_WRITE, (type), (nr), (_IOC_TYPECHECK(size)))
#define _IOR_BAD(type, nr, size) _IOC(_IOC_READ, (type), (nr), sizeof(size))
#define _IOW_BAD(type, nr, size) _IOC(_IOC_WRITE, (type), (nr), sizeof(size))
#define _IOWR_BAD(type, nr, size) _IOC(_IOC_READ | _IOC_WRITE, (type), (nr), sizeof(size))

#define _IOC_DIR(nr) (((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
#define _IOC_TYPE(nr) (((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)
#define _IOC_NR(nr) (((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
#define _IOC_SIZE(nr) (((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)

#define IOC_IN (_IOC_WRITE << _IOC_DIRSHIFT)
#define IOC_OUT (_IOC_READ << _IOC_DIRSHIFT)
#define IOC_INOUT ((_IOC_WRITE | _IOC_READ) << _IOC_DIRSHIFT)
#define IOCSIZE_MASK (_IOC_SIZEMASK << _IOC_SIZESHIFT)
#define IOCSIZE_SHIFT (_IOC_SIZESHIFT)

#define TCGETA 0x5401
#define TCSETA 0x5402  
#define TCSETAW 0x5403
#define TCSETAF 0x5404

#define TCSBRK 0x5405
#define TCXONC 0x5406
#define TCFLSH 0x5407

#define TCGETS 0x540d
#define TCSETS 0x540e
#define TCSETSW 0x540f
#define TCSETSF 0x5410

#define TIOCEXCL 0x740d							 
#define TIOCNXCL 0x740e							 
#define TIOCOUTQ 0x7472							 
#define TIOCSTI 0x5472							 
#define TIOCMGET 0x741d							 
#define TIOCMBIS 0x741b							 
#define TIOCMBIC 0x741c							 
#define TIOCMSET 0x741a							 
#define TIOCPKT 0x5470							 
#define TIOCPKT_DATA 0x00						 
#define TIOCPKT_FLUSHREAD 0x01					 
#define TIOCPKT_FLUSHWRITE 0x02					 
#define TIOCPKT_STOP 0x04						 
#define TIOCPKT_START 0x08						 
#define TIOCPKT_NOSTOP 0x10						  
#define TIOCPKT_DOSTOP 0x20						  
#define TIOCPKT_IOCTL 0x40						  
#define TIOCSWINSZ _IOW('t', 103, struct winsize) 
#define TIOCGWINSZ _IOR('t', 104, struct winsize) 
#define TIOCNOTTY 0x5471						  
#define TIOCSETD 0x7401
#define TIOCGETD 0x7400

#define FIOCLEX 0x6601
#define FIONCLEX 0x6602
#define FIOASYNC 0x667d
#define FIONBIO 0x667e
#define FIOQSIZE 0x667f

#define TIOCGLTC 0x7474				  
#define TIOCSLTC 0x7475				  
#define TIOCSPGRP _IOW('t', 118, int) 
#define TIOCGPGRP _IOR('t', 119, int) 
#define TIOCCONS _IOW('t', 120, int)  

#define FIONREAD 0x467f
#define TIOCINQ FIONREAD

#define TIOCGETP 0x7408
#define TIOCSETP 0x7409
#define TIOCSETN 0x740a 

#define TIOCSCTTY 0x5480 
#define TIOCGSOFTCAR 0x5481
#define TIOCSSOFTCAR 0x5482
#define TIOCLINUX 0x5483
#define TIOCGSERIAL 0x5484
#define TIOCSSERIAL 0x5485
#define TCSBRKP 0x5486 
#define TIOCSERCONFIG 0x5488
#define TIOCSERGWILD 0x5489
#define TIOCSERSWILD 0x548a
#define TIOCGLCKTRMIOS 0x548b
#define TIOCSLCKTRMIOS 0x548c
#define TIOCSERGSTRUCT 0x548d  
#define TIOCSERGETLSR 0x548e   
#define TIOCSERGETMULTI 0x548f 
#define TIOCSERSETMULTI 0x5490 
#define TIOCMIWAIT 0x5491	   
#define TIOCGICOUNT 0x5492	   

/**
 * @brief ioctl
 * 
 * @param fd 
 * @param cmd 
 * @param ... 
 * @return int 
 */
int ioctl(int fd, unsigned int cmd, ...);