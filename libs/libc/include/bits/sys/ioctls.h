#pragma once

/* TTY */
#define TIOCGPGRP 0x0101
#define TIOCSPGRP 0x0102
#define TCGETS 0x0103
#define TCSETS 0x0104
#define TCSETSW 0x0105
#define TCSETSF 0x0106

/* BGA */
#define BGA_SWAP_BUFFERS 0x0101
#define BGA_GET_HEIGHT 0x0102
#define BGA_GET_WIDTH 0x0103