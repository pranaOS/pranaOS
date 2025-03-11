#import <Foundation/Foundation.h>

void first() {
    NSLog(@"FIRST");
}


void second() {
    NSLog(@"SECOND");
}

int main() {
    int x = 1;
    int y = 1;
    
    if (x == y) {
        first();
    } else {
        second();
    }
    
    return 0;
}
