#ifndef __misc_h__
#define __misc_h__

#include <8051.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#define nop() __asm nop __endasm

#define LED_PORT P1

#endif

