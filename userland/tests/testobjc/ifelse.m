#include <libfoundation/NSObject.h>
#include <libobjc/helpers.h>
#include <stdio.h>

int main() {
    int a = 100;

    if (a < 20) {
        printf("a is less than 20\n");
    } else {
        printf("a is not less than 20\n");
    }

    printf("value of a is : %d\n", a);

    return 0;
}