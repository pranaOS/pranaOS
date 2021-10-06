#include <stdio.h>

@implementation NSMyObject

@synthesize lifetimeTracked;

- (instancetype) init {
    self = [super init];
    if (!self) {
        return nil;
    }

    return self;
}

- (void) print {
    printf("Hello!\n");
    fflush(stdout);
}

@end