#include <stdio.h>

int main(int argc, char** argv)
{
    int c;
    
    for(int i = 1; i < argc; i++){
        printf("%s", argv[i]);
        c = getchar();

        if(i < argc - 1){ 
            putchar(c);    
        }
    }

    return 0;
}