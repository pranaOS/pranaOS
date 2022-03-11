#include "keyboard.h"


using namespace Kernel;

struct keyboard_status {
    unsigned char A : 1;
    unsigned char B : 2;
    unsigned char C : 3;
} keyboard_status_t;

void keyboardStatus() {
}
