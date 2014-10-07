#ifndef STDAFX_H
#define STDAFX_H

#pragma once

typedef unsigned int UINT;

#include <stdlib.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif


//#include <windows.h>
//#pragma comment(lib,"Imm32.lib")
//#pragma comment(lib,"winmm.lib")
//#pragma comment(lib,"Ws2_32.lib")
//#pragma comment(lib,"version.lib")

#endif

#if defined(LINUX)

#define LONG long
#define DWORD unsigned int
#define CHAR char
#define UCHAR unsigned char

void inline Beep(unsigned int dwFreq,unsigned int dwDuration)
{
}

struct RECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
};

#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <assert.h>

#include <vector>
#include <list>
#include <deque>
using namespace std;

//#include "xtimer.h"
//#include "xeng.h"

struct RECT
{
    int left, right, top, bottom;
};

void xfBeep(int freq = 1000, int duration = 100);

bool XFAIL(int ret);

// TODO: need proper template
float SQR(float val);

bool xfBetween(double val, double val1, double val2);

bool xfBetweenExcl(double val, double val1, double val2);

#endif
