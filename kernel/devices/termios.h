#ifndef TERMIOS_H
#define TERMIOS_H

#include <stdint.h>

typedef unsigned char cc_t;
typedef unsigned int speed_t;
typedef unsigned int tcflag_t;

struct sgttyb {
	char sg_ispeed;
	char sg_ospeed;
	char sg_erase;
	char sg_kill;
	short sg_flags;
};

struct tchars {
	char t_intrc;
	char t_quitc;
	char t_startc;
	char t_stopc;
	char t_eofc;
	char t_brkc;
};

struct ltchars {
	char t_suspc;
	char t_dsuspc;
	char t_rprntc;
	char t_flushc;
	char t_werasc;
	char t_lnextc;
};

struct winsize {
	unsigned short ws_row;	  
	unsigned short ws_col;	  
	unsigned short ws_xpixel; 
	unsigned short ws_ypixel; 
};

#define NCCS 19

struct termios {
	tcflag_t c_iflag; 
	tcflag_t c_oflag; 
	tcflag_t c_cflag; 
	tcflag_t c_lflag; 
	cc_t c_line;	  
	cc_t c_cc[NCCS];  
};


#endif 