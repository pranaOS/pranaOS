//
//  NSblocks.m
//  libobjc
//
//  Created by KrisnaPranav on 06/03/22.
//

#import <stdbool.h>
#import <stdlib.h>
#import <stdio.h>
#import <string.h>

#undef memmove

static bool isGC = false;

/**
 * @brief NS Concrete values
 */
void * _NSConcreteStackBlock[32];
void * _NSConcreteMallocBlock[32];
void * _NSConcreteAutoBlock[32];
void * _NSConcreteFinalizingBlock[32];
void * _NSConcreteGlobalBlock[32];
void * _NSConcreteWeakBlockVariable[32];
