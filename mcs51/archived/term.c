#include "term.h"

static uchar x, y;
uchar data term_tabsize;

void term_init(void)
{
    x = y = 0;
    term_tabsize = TABSIZE_DEFAULT;
}
void term_cls(void)
{
    GXM12864_cls();
	x = y = 0;
}
void term_reset(void)
{
    x = y = 0;
}
static void term_newline(void)
{
    if (y >= (GXM12864_SCREEN_ROWS - 1))
    {
        GXM12864_moveup();
    }
    else
    {
        y++;
    }
    x = 0;
}
void term_putchar(uchar ch)
{
    uchar status = 0;

    //Text mode dealer
    if (x >= GXM12864_SCREEN_COLS)
    {
        term_newline();
    }

    switch (ch)
    {
        //Control chars
        case 0x08:
            if (0 == x)
            {
                if (0 > y)
                {
                    y--;
                    x = GXM12864_SCREEN_COLS - 1;
                }
            }
            else
            {
                x--;
            }
        break;
        case 0x09:
            x += term_tabsize - (x % term_tabsize);
        break;
        case 0x0A:
            x = 0;
        break;
        case 0x0B:
            if (y >= 7)
            {
                GXM12864_moveup();
            }
            else
            {
                y++;
            }
        break;
		case 0x0C:
			term_cls();
		break;
        case 0x0D:
            term_newline();
        break;

        //Print chars
        default:
            status = GXM12864_drawchar(ch, y, x);
        break;
    }

    if (status)
    {
        x++;
    }
}
uchar term_proc(uchar rcv)
{
    switch (rcv)
    {
        case CHAR_SIG_GRAPH:
        case CHAR_SIG_LED:
            return 0;
    }

    term_putchar(rcv);
    graph_reset();
    return 1;
}
