//
//  protocol.m
//  libobjc
//
//  Created by KrisnaPranav on 09/03/22.
//

#import "runtime/NSObject.h"
#import "runtime/objc.h"
#import "runtime/runtime.h"

@interface __incompleteProtocol : NSObject
@end

@implementation __incompleteProtocol
+ (void) load {
}
@end
