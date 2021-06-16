#include "com.h"
#include "gxm12864.h"

#define COMMAND_NULL 0x00
#define COMMAND_CLEAR_SCREEN 0x01
#define COMMAND_SET_LED 0x02
#define COMMAND_DRAW_SCREEN 0x80

static uchar __data
	current_command=COMMAND_NULL,
	screen_x=0,
	screen_y=0,
	screen_buf[8],
	screen_buf_idx=0;

void procCommand(uchar cmd){
	//12864 clear screen
	if(COMMAND_CLEAR_SCREEN==cmd){
		GXM12864_cls();
		current_command=COMMAND_NULL;
		return;
	}
	//Setup LED
	if(COMMAND_SET_LED==cmd){
		current_command=cmd;
		return;
	}
	//12864 set draw position & start receiving data
	if(COMMAND_DRAW_SCREEN & cmd){
		current_command=COMMAND_DRAW_SCREEN;
		screen_x=(cmd&0x0f);
		screen_y=(cmd>>4)&0x07;
		screen_buf_idx=0;
		return;
	}
	//Ignore invalid commands
	current_command=COMMAND_NULL;
	screen_x=0;
	screen_y=0;
	screen_buf_idx=0;
}
void procData(uchar ch){
	//Setup LED
	if(COMMAND_SET_LED==current_command){
		current_command=COMMAND_NULL;
		LED_PORT=~ch;
		return;
	}
	//Screen set data
	if(COMMAND_DRAW_SCREEN==current_command){
		screen_buf[screen_buf_idx]=ch;
		screen_buf_idx++;
		if(screen_buf_idx>=8){
			current_command=COMMAND_NULL;
			GXM12864_wpattern(screen_buf, screen_x, screen_y);
		}
		return;
	}
}
void procChar(uchar ch){
	if(COMMAND_NULL==current_command){
		procCommand(ch);
	}else{
		procData(ch);
	}
}
void main()
{
  uchar __data com_read;

  //Init watchdog
  __asm__("mov 0xE1,#0b00111110");

  GXM12864_init();
	com_init();
	LED_PORT=0xFF;
  GXM12864_cls();
  while(1){
    //Feed watchdog
    __asm__("orl 0xE1,#0b00010000");
    if(!com_rcv(&com_read)){
			continue;
    }
		procChar(com_read);
  }
}

