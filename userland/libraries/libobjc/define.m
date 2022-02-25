#import <Foundation/Foundation.h>

#define LENGTH 10
#define WIDTH 5
#define NEWLINE '\n'

int main() {
    int area;
    area = LENGTH * WIDTH;
    NSLog(@"value of area: %d", area);
    NSLog(@"%c", NEWLINE);

    return 0;
}