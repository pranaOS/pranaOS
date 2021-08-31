#include <stdio.h>
#include <signal.h>

int main(int argc, char **argv)
{
    printf("rebooting");

    return kill(1, SIGHUP);
}