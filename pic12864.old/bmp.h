#ifndef __BMP_H__
#define __BMP_H__

#define WIN32

#ifdef WIN32
    typedef unsigned char uint8_t;
	typedef short uint16_t;
	typedef int uint32_t;
#else
	#include <linux/types.h>
    typedef u_int8_t  uint8_t;
	typedef u_int16_t uint16_t;
	typedef u_int32_t uint32_t;
#endif

#define WIDTH_LCD 128
#define HEIGHT_LCD 64

//bmp header
typedef struct bmpinfo_items
{
	uint32_t fsize;
	uint32_t reserved;
	uint32_t description;
	uint32_t size;
	uint32_t width;
	uint32_t height;
	uint16_t biplanes;
	uint16_t bitcount;
	uint32_t compressed;
	uint32_t sizeimage;
	uint32_t Xppm;
	uint32_t Yppm;
	uint32_t used;
	uint32_t important;
}bmpinfo;

int loadBMP(char *path, uint8_t *buf);
void convBMP_12864(
    uint8_t bufpic[HEIGHT_LCD][WIDTH_LCD / 8],
    uint8_t buf12864[HEIGHT_LCD/8][WIDTH_LCD]);
void outHexText(uint8_t *buf12864);

#endif
