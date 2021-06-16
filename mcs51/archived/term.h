#ifndef __terminal_h__
#define __terminal_h__

#include "misc.h"
#include "gxm12864.h"

#define CHAR_SIG_GRAPH 0x11
#define CHAR_SIG_LED 0x12

#define TABSIZE_DEFAULT 4
#define LED_PORT P1

//Text terminal
extern uchar data term_tabsize;
void term_init(void);
void term_reset(void);
void term_cls(void);
void term_putchar(uchar ch);
uchar term_proc(uchar rcv);

//Graphic terminal
void graph_init(void);
void graph_reset(void);
uchar graph_proc(uchar rcv);

//LED control
void led_init(void);
uchar led_proc(uchar value);

#endif
