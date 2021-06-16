#include "term.h"

static uchar data x, y, rcv_cnt, rcv_buf[8];
static uchar data isgraph;

void graph_init()
{
    x = y = 0;
    rcv_cnt = 0;
    isgraph = 0;
}
void graph_reset()
{
    graph_init();
}
uchar graph_draw(uchar img_dat)
{
    rcv_buf[rcv_cnt] = img_dat;
    rcv_cnt++;

    //Data not completed
    if (rcv_cnt < 8)
    {
        return 0;
    }

    GXM12864_wpattern(rcv_buf, x, y);
    rcv_cnt = 0;

    //Get next pos
    if (x >= (GXM12864_SCREEN_COLS - 1))
    {
        if (y >= (GXM12864_SCREEN_ROWS - 1))
        {
            x = y = 0;
        }
        else
        {
            x = 0;
            y++;
        }
    }
    else
    {
        x++;
    }

    return 1;
}
uchar graph_proc(uchar rcv)
{
    if (CHAR_SIG_GRAPH == rcv && !isgraph)
    {
        term_reset();
        isgraph = 1;
        return 1;
    }
    else if (isgraph)
    {
        if (graph_draw(rcv))
        {
            isgraph = 0;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}
