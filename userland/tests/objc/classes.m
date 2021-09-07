/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#include <libfoundation/NSObject.h>
#include <libobjc/helpers.h>
#include <stdio.h>

@interface Box:NSObject {
   double length;    
   double breadth;   
   double height;    
}

@property(nonatomic, readwrite) double height;  
-(double) volume;
@end

@implementation Box

@synthesize height; 

-(id)init {
   self = [super init];
   length = 1.0;
   breadth = 1.0;
   return self;
}

-(double) volume {
   return length*breadth*height;
}

@end

int main() {
   NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];    
   Box *box1 = [[Box alloc]init];    
   Box *box2 = [[Box alloc]init];    

   double volume = 0.0;             
 
   box1.height = 5.0; 

   box2.height = 10.0;
  
   volume = [box1 volume];
   printf(@"Volume of Box1 : %f", volume);
   
   volume = [box2 volume];
   printf(@"Volume of Box2 : %f", volume);
   
   [pool drain];
   return 0;
}
