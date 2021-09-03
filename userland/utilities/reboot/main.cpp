#include <stdio.h>
#include <signal.h>

int main()
{
    printf("rebooting...");
    // return kill(1, SIGHUP);
    return 0;
}
