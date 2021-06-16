#include "com.h"

static uchar __data com_buf[COM_BUF_LEN];
static uchar __data *com_buf_head, *com_buf_tail, com_buf_size;

inline void com_init(void)
{
	SCON = 0x50;//设置串口8位UART，波特率由T1产生，允许接收
	PCON = 0x80;//SMOD为1，波特率不加倍
	SBUF = 0;
	TMOD = 0x20;//定时器1设置为方式2，8位自动重装载
	ES = 1;     //允许串口中断
	EA = 1;     //开总中断
	TH1 = 0xff; //设定重装载值
	TL1 = 0xff;
	TR1 = 1;    //启动定时器产生波特率

  //Init com buffer
  com_buf_head = com_buf_tail = com_buf;
  com_buf_size = 0;
}
inline static uchar com_buf_put(uchar com_read)
{
    EA = 0;
    if (COM_BUF_LEN == com_buf_size)
    {
        EA = 1;
        return 0;
    }

    *com_buf_head = com_read;

    //Update buffer info
    if (com_buf_head >= (com_buf + COM_BUF_LEN - 1))
    {
        com_buf_head = com_buf;
    }
    else
    {
        com_buf_head++;
    }
    com_buf_size++;
    EA = 1;

    return 1;
}
inline uchar com_rcv(uchar *buf)
{
    EA = 0;
    if (0 == com_buf_size)
    {
        EA = 1;
        return 0;
    }

    *buf = *com_buf_tail;

    //Update buffer info
    if (com_buf_tail >= (com_buf + COM_BUF_LEN - 1))
    {
        com_buf_tail = com_buf;
    }
    else
    {
        com_buf_tail++;
    }
    com_buf_size--;
    EA = 1;

    return 1;
}

void serial_receiver(void) __interrupt 4{
    if (RI)
    {
        RI = 0;
        com_buf_put(SBUF);
    }
    if (TI)
    {
        TI = 0;
    }
}
