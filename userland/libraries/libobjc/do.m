#import <Foundation/Foundation.h>

int main() {
    int a = 10;

    do {
        NSLog(@"value of a: %d\n", a);
        a = a + 1;
    } while (a < 20);

    return 0;
}