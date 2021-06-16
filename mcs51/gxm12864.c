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
static inline void GXM12864_start_cmdwrite(){
  GXM12864_CTRLOUT &= ~(GXM12864_ID|GXM12864_RW); //内容为指令,进行写操作
}
static inline void GXM12864_start_datawrite(){
  GXM12864_CTRLOUT |= GXM12864_ID; //内容为数据
  GXM12864_CTRLOUT &= ~GXM12864_RW; //进行写操作
}
static inline void GXM12864_write(uchar data_buffer){
	GXM12864_DATAOUT=data_buffer;	//将数据加载到端口上
	GXM12864_CTRLOUT |= GXM12864_E;	//高电平使能，使能后完成所要的操作
  GXM12864_CTRLOUT &= ~GXM12864_E; //操作已完成，应将使能信号降为低电平
}
//***************************************************//
void GXM12864_init(void)
{
	GXM12864_CTRLOUT &= ~(GXM12864_CSA|GXM12864_CSB);
	GXM12864_CTRLOUT = 0x2c;
  GXM12864_start_cmdwrite();

  //Init left screen
	GXM12864_CTRLOUT |= GXM12864_CSA;
	GXM12864_CTRLOUT &= ~GXM12864_CSB;
  GXM12864_write(0X00);
  GXM12864_write(0X3f);
  GXM12864_write(0Xc0);
  GXM12864_write(0X0b8);
  GXM12864_write(0X40);

  //Init right screen
	GXM12864_CTRLOUT &= ~GXM12864_CSA;
	GXM12864_CTRLOUT |= GXM12864_CSB;
  GXM12864_write(0X00);
  GXM12864_write(0X3f);
  GXM12864_write(0Xc0);
  GXM12864_write(0X0b8);
  GXM12864_write(0X40);
}
void GXM12864_cls()
{
	uchar i, j, t;

  for (t = 0; t < 2; t++)
  {
    if(0==t){ //Left screen
      GXM12864_CTRLOUT |= GXM12864_CSA;
      GXM12864_CTRLOUT &= ~GXM12864_CSB;
    }else{   //Right screen
      GXM12864_CTRLOUT &= ~GXM12864_CSA;
      GXM12864_CTRLOUT |= GXM12864_CSB;
    }

    for(j=0;j<8;j++){
      GXM12864_wait();
      GXM12864_start_cmdwrite();
      GXM12864_write(j | 0x0b8);	

      //GXM12864_wait();
      GXM12864_start_datawrite();
      for (i=0;i<64;i++){
        GXM12864_write(0x00);
      }
    }
  }
}
inline void GXM12864_wpattern(uchar __data *imgdata, uchar col, uchar row)
{
	uchar i, page_addr, col_addr;

  //Specify col
  if(col & 0xf8){ //Right screen
    GXM12864_CTRLOUT &= ~GXM12864_CSA;
    GXM12864_CTRLOUT |= GXM12864_CSB;
    col_addr = col & 0x07;
  }else{ //Left sscreen
    GXM12864_CTRLOUT |= GXM12864_CSA;
    GXM12864_CTRLOUT &= ~GXM12864_CSB;
    col_addr = col;
  }

  GXM12864_wait();
  GXM12864_start_cmdwrite();
	GXM12864_write((col_addr<<3) | 0x40);
	GXM12864_write(row | 0x0b8);

  //GXM12864_wait();
  GXM12864_start_datawrite();
	for(i = 0; i < 8; i++){
    GXM12864_write(imgdata[i]);
	}
}

