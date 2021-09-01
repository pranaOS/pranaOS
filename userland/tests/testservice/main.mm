#include <libfoundation/NSObject.h>
#include <libobjc/helpers.h>
#include <stdio.h>

int start_platform_service(int state);

@interface Service: NSObject
@end

@implementation Service

- (void) startService {
    int state = 1;
    int result = start_platform_service(state);
    if (result == 1) {
        printf("Success");
    } else {
        printf("Failure");
    }
}

@end

@implementation ServiceTest

static BOOL start_platform_service_mock_get_called = NO;

int start_platform_service(int state) {
    start_platform_service_mock_get_called = YES;
    return 0;
}

@end