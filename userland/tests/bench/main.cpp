#include "common.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

char* bench_name;
int bench_pno = -1;
int bench_no = 0;
int bench_run = 0;
timeval_t tv, ttv;
timezone_t tz;

void bench_kernel()
{
    RUN_BENCH("FORK", 3)
    {
        for (int i = 0; i < 20; i++) {
            int pid = fork();
            if (pid < 0) {
                return;
            }
            if (pid) {
                wait(pid);
            } else {
                exit(0);
            }
        }
    }
}


void loop_function()
{
    for (;;) {
        bench_kernel();
        bench_pngloader();
        printf("[BENCH END TEST]\n\n");
        fflush(stdout);
        return 0;
    }
}

int main(int argc, char** argv)
{
    loop_function();
}