#include <libfoundation/NSObject.h>
#include <libobjc/helpers.h>
#include <stdio.h>

@interface SampleClass : NSObject {
@public
    int last_val;
}
+ (void)sampleMethod;
- (void)sampleMethod:(int)val;
- (int)get_last;
@end

@implementation SampleClass

+ (void)sampleMethod
{
    printf("Calling static: Hello, from Obj-C!");
}

- (void)sampleMethod:(int)val
{
    last_val = val;
    printf("Calling method: Hello, Obj-C! %d", val);
}

- (int)get_last
{
    return last_val;
}

@end

int main()
{

    int a = 100;

    if (a < 100) {
        printf("a is less than 20\n");
    } else {
        printf("a is not less than 20\n");
    }

    printf("value of a is: %d\n", a);

    id objectAlloc = [[SampleClass alloc] init];
    [objectAlloc sampleMethod:22];
    [SampleClass sampleMethod];
    printf("Last called with %d", [objectAlloc get_last]);
    return 0;
}