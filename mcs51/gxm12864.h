#ifndef __gxm12864_h__
#define __gxm12864_h__

#include "misc.h"

//Functions
void GXM12864_init(void);
void GXM12864_cls();
void GXM12864_wpattern(uchar __data *imgdata, uchar col, uchar row);

#endif
