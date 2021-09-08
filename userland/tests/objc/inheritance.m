/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#import <libfoundation/NSObject.h>
#import <libobjc/helpers.h>
#import <stdio.h>

@interface Person : NSObject {
   NSString *personName;
   NSInteger personAge;
}

- (id)initWithName:(NSString *)name andAge:(NSInteger)age;
- (void)print;

@end

@implementation Person

- (id)initWithName:(NSString *)name andAge:(NSInteger)age {
   personName = name;
   personAge = age;
   return self;
}

- (void)print {
   printf("Name: %", personName);
   printf("Age: %ld", personAge);
}

@end

@interface Employee : Person {
   NSString *employeeEducation;
}

- (id)initWithName:(NSString *)name andAge:(NSInteger)age 
  andEducation:(NSString *)education;
- (void)print;
@end

@implementation Employee

- (id)initWithName:(NSString *)name andAge:(NSInteger)age 
   andEducation: (NSString *)education {
      personName = name;
      personAge = age;
      employeeEducation = education;
      return self;
   }

- (void)print {
   printf("Name: %", personName);
   printf("Age: %ld", personAge);
   printf("Education: %", employeeEducation);
}

@end

int main(int argc, const char * argv[]) {
   NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];        
   printf("Base class Person Object");
   Person *person = [[Person alloc]initWithName:"test" andAge:5];
   [person print];
   printf("Inherited Class Employee Object");
   Employee *employee = [[Employee alloc]initWithName:"test" 
   andAge:5 andEducation:"MBA"];
   [employee print];        
   [pool drain];
   return 0;
}