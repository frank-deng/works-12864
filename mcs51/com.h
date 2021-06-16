#ifndef __com_h__
#define __com_h__

#include "misc.h"

void com_init(void);
uchar com_rcv(uchar *buf);
#define COM_BUF_LEN 64

void serial_receiver(void) __interrupt 4;

#endif

