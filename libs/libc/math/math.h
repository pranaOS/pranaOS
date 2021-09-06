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