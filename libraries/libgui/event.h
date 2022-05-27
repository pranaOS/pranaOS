#ifndef GUI_EVENT_H
#define GUI_EVENT_H

struct xevent_type {
    XMOTION_EVENT,
    XBUTTON_EVENT,
    XKEY_EVENT,
};

struct xevent {
    enum xevent_type type;
    char data[32];
};

#endif