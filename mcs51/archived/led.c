#include "term.h"

static uchar data isled;

void led_init()
{
    isled = 0;
    LED_PORT = 0xFF;
}
uchar led_proc(uchar rcv)
{
    if (CHAR_SIG_LED == rcv && !isled)
    {
        isled = 1;
        return 1;
    }
    else if (isled)
    {
        LED_PORT = ~rcv;
        isled = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}
