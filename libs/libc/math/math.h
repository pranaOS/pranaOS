/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libc/__libc__.h>
#include <bits/abs.h>
#include <bits/float.h>
#include <bits/pi.h>

__BEGIN_HEADER

extern int signgam;

double j0(double);
double j1(double);
double jn(int, double);

void sincos(double, double *, double *);
void sincosf(float, float *, float *);
void sincosl(long double, long double *, long double *);

double exp10(double);
float exp10f(float);
long double exp10l(long double);

double pow10(double);
float pow10f(float);
long double pow10l(long double);

double acos(double x);
float acosf(float x);
long double acosl(long double x);
double asin(double x);
float asinf(float x);
long double asinl(long double x);
double atan(double x);
float atanf(float x);
long double atanl(long double x);
double atan2(double x, double y);
float atan2f(float x, float y);
long double atan2l(long double x, long double y);
double cos(double x);
float cosf(float x);
long double cosl(long double x);
double sin(double x);
float sinf(float x);
long double sinl(long double x);
double tan(double x);
float tanf(float x);
long double tanl(long double x);

double acosh(double x);
float acoshf(float x);
long double acoshl(long double x);

double asinh(double x);
float asinhf(float x);
long double asinhl(long double x);

double rint(double x);
float rintf(float x);
long double rintl(long double x);

long lrint(double x);
long lrintf(float x);
long lrintl(long double x);

long long llrint(double x);
long long llrintf(float x);
long long llrintl(long double x);

double round(double x);
float roundf(float x);
long double roundl(long double x);

long lround(double x);
long lroundf(float x);
long lroundl(long double x);

long long llround(double x);
long long llroundf(float x);
long long llroundl(long double x);

double trunc(double x);
float truncf(float x);
long double truncl(long double x);

double fma(double, double, double);
float fmaf(float, float, float);
long double fmal(long double, long double, long double);

__END_HEADER