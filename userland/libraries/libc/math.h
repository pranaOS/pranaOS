/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <__libc__.h>
#include <bits/abs.h>
#include <bits/float.h>
#include <bits/pi.h>

__BEGIN_HEADER

#if defined(_XHJ_OPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE) || defined(_SKIFT_SOURCE)
#    define M_E 2.7182818284590452354       
#    define M_LOG2E 1.4426950408889634074   
#    define M_LOG10E 0.43429448190325182765 
#    define M_LN2 0.69314718055994530942    
#    define M_LN10 2.30258509299404568402   

#    define M_SQRT2 1.41421356237309504880   
#    define M_SQRT1_2 0.70710678118654752440 

extern int signgam;

double j0(double);
double j1(double);
double jn(int, double);

double y0(double);
double y1(double);
double yn(int, double);

#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE) || defined(_SKIFT_SOURCE)
#    define HUGE 3.40282346638528859812e+38F

double drem(double, double);
float dremf(float, float);

int finite(double);
int finitef(float);

double scalb(double, double);
float scalbf(float, float);

double significand(double);
float significandf(float);

double lgamma_r(double, int *);
float lgammaf_r(float, int *);

float j0f(float);
float j1f(float);
float jnf(int, float);

float y0f(float);
float y1f(float);
float ynf(int, float);

#endif