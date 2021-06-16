#include "gxm12864.h"

#define GXM12864_DATAOUT P0
#define GXM12864_CTRLOUT P2

#define GXM12864_SCREEN_W 128
#define GXM12864_SCREEN_H 64
#define GXM12864_SCREEN_COLS (GXM12864_SCREEN_W / 8)
#define GXM12864_SCREEN_ROWS (GXM12864_SCREEN_H / 8)

#define GXM12864_RST 0x20
#define GXM12864_CSA 0x08
#define GXM12864_CSB 0x10
#define GXM12864_ID 0x01
#define GXM12864_RW 0x02
#define GXM12864_E 0x04

static void GXM12864_wait(void)
{
	uchar busy_bit=0;

  GXM12864_CTRLOUT &= ~GXM12864_ID; //内容为指令
  GXM12864_CTRLOUT |= GXM12864_RW; //进行读操作（确认设备是否忙）
	do
	{
		GXM12864_DATAOUT=0x0ff;	//读操作前初始化端口
		GXM12864_CTRLOUT |= GXM12864_E;	//高电平使能，使能后完成所要的操作
		nop();	//等待设备完成操作
		busy_bit=GXM12864_DATAOUT;	//读取状态，最高位表示液晶屏是否忙，如果忙则不能写入
    GXM12864_CTRLOUT &= ~GXM12864_E; //操作已完成，应将使能信号降为低电平
	}
	while(0x80 & busy_bit);
}
static void GXM12864_wcmd(uchar command)
{
	GXM12864_wait();	//等待设备空闲

	//设备已空闲，开始写入指令
  GXM12864_CTRLOUT &= ~GXM12864_ID; //内容为指令
  GXM12864_CTRLOUT &= ~GXM12864_RW; //进行写操作
	GXM12864_DATAOUT=command;	//将数据加载到端口上
	GXM12864_CTRLOUT |= GXM12864_E;	//高电平使能，使能后完成所要的操作
	nop();	//等待设备完成操作
	nop();
  GXM12864_CTRLOUT &= ~GXM12864_E; //操作已完成，应将使能信号降为低电平
}
static void GXM12864_wdat(uchar data_buffer)
{
	GXM12864_wait();	//等待设备空闲

	//设备已空闲，开始写入数据
  GXM12864_CTRLOUT |= GXM12864_ID; //内容为数据
  GXM12864_CTRLOUT &= ~GXM12864_RW; //进行写操作
  /*
	GXM12864_ID=1;	//内容为数据
	GXM12864_RW=0;	//进行写操作
  */
	GXM12864_DATAOUT=data_buffer;	//将数据加载到端口上
	GXM12864_CTRLOUT |= GXM12864_E;	//高电平使能，使能后完成所要的操作
	nop();	//等待设备完成操作
	nop();
  GXM12864_CTRLOUT &= ~GXM12864_E; //操作已完成，应将使能信号降为低电平
}
static uchar GXM12864_rdat()
{
  uchar dat_temp;
	GXM12864_wait();	//等待设备空闲

	//设备已空闲，开始写入数据
  GXM12864_CTRLOUT |= GXM12864_ID; //内容为数据
  GXM12864_CTRLOUT |= GXM12864_RW; //进行读操作（确认设备是否忙）
	GXM12864_DATAOUT = 0x0ff;	//读操作前初始化端口
	GXM12864_CTRLOUT |= GXM12864_E;	//高电平使能，使能后完成所要的操作
  nop();	//等待设备完成操作
  nop();	//等待设备完成操作
  dat_temp = GXM12864_DATAOUT;	//读取状态，最高位表示液晶屏是否忙，如果忙则不能写入
  GXM12864_CTRLOUT &= ~GXM12864_E; //操作已完成，应将使能信号降为低电平
  return dat_temp;
}
//***************************************************//
void GXM12864_init(void)
{
	GXM12864_CTRLOUT &= ~GXM12864_CSA;
	GXM12864_CTRLOUT &= ~GXM12864_CSB;
	GXM12864_CTRLOUT = 0x2c;

  //Init left screen
	GXM12864_CTRLOUT |= GXM12864_CSA;
	GXM12864_CTRLOUT &= ~GXM12864_CSB;
	//GXM12864_CSA = 1;
	//GXM12864_CSB = 0;
	GXM12864_wcmd(0X00);
	GXM12864_wcmd(0x3f);
	GXM12864_wcmd(0Xc0);
	GXM12864_wcmd(0X0b8);
	GXM12864_wcmd(0X40);

  //Init right screen
	//GXM12864_CSA = 0;
	//GXM12864_CSB = 1;
	GXM12864_CTRLOUT &= ~GXM12864_CSA;
	GXM12864_CTRLOUT |= GXM12864_CSB;
	GXM12864_wcmd(0X00);
	GXM12864_wcmd(0x3f);
	GXM12864_wcmd(0Xc0);
	GXM12864_wcmd(0X0b8);
	GXM12864_wcmd(0X40);
}
void GXM12864_cls()
{
	uchar i, j, t, page_address;

  for (t = 0; t < 2; t++)
  {
    if (0 == t) //Left screen
    {
      GXM12864_CTRLOUT |= GXM12864_CSA;
      GXM12864_CTRLOUT &= ~GXM12864_CSB;
    }
    else    //Right screen
    {
      GXM12864_CTRLOUT &= ~GXM12864_CSA;
      GXM12864_CTRLOUT |= GXM12864_CSB;
    }

    for(j=0;j<8;j++)
    {
      page_address = 0x00;
      page_address = j;
      page_address |= 0x0b8;
      GXM12864_wcmd(page_address);	

      for (i=0;i<64;i++)
      {
        GXM12864_wdat(0x00);
      }
    }
    GXM12864_wcmd(0xB8);
    GXM12864_wcmd(0x40);
  }
}
void GXM12864_wpattern(uchar __data *imgdata, uchar col, uchar row)
{
	uchar i, page_addr, col_addr;

  //Specify col
	col_addr = 0;
  if (col < (GXM12864_SCREEN_COLS >> 1))   //左半屏
  {
    GXM12864_CTRLOUT |= GXM12864_CSA;
    GXM12864_CTRLOUT &= ~GXM12864_CSB;
    col_addr = col;
  }
  else    //右半屏
  {
    GXM12864_CTRLOUT &= ~GXM12864_CSA;
    GXM12864_CTRLOUT |= GXM12864_CSB;
    col_addr = col - (GXM12864_SCREEN_COLS >> 1);
  }
  col_addr <<= 3;
  col_addr |= 0x40;
	GXM12864_wcmd(col_addr);

    //Specify row
	page_addr = 0;
  page_addr = row;
  page_addr |= 0x0b8; //写屏幕指令
	GXM12864_wcmd(page_addr);

	for(i = 0; i < 8; i++)
	{
    GXM12864_wdat(imgdata[i]);
	}

  //Reset screen
	GXM12864_wcmd(0xB8);
	GXM12864_wcmd(0x40);
}

