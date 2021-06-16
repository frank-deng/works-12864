#include <stdio.h>
#include <string.h>
#include "bmp.h"

int loadBMP(char *path, uint8_t *buf)
{
	FILE *fbmp;
	uint16_t flag;
	bmpinfo info;
	int i;

	//Is BMP file?
	fbmp = fopen(path, "rb");
	fseek(fbmp, 0, SEEK_SET);
	fread(&flag, sizeof(uint16_t), 1, fbmp);
	if (0x4d42 != flag)
	{
		fclose(fbmp);
		fprintf(stderr, "Not a BMP file.\n");
		return 0;
	}

	//Get info of BMP
	fseek(fbmp, 2, SEEK_SET);
	fread(&info, sizeof(bmpinfo), 1, fbmp);
	if (WIDTH_LCD  != info.width
        || HEIGHT_LCD != info.height
        || 1 != info.bitcount
        || 0 != info.compressed)
	{
		fclose(fbmp);
		fprintf(stderr, "Wrong Format.\n");
		return 0;
	}

	//Read file
	for (i = 0; i < info.height; i++)
	{
		fseek(
			fbmp,
			info.description + (WIDTH_LCD / 8 * i),
			SEEK_SET
		);
		fread(
			buf + (WIDTH_LCD / 8 * (info.height - 1 - i)),
			sizeof(uint8_t),
			WIDTH_LCD / 8,
			fbmp
		);
	}

	//finish
	fclose(fbmp);
	return 1;
}

//Converter
static void _rotateData(uint8_t *src, uint8_t *dest)
{
	int i,j;
	uint8_t mask=0x01, data;

	for (i = 0; i < 8;i++)
	{
		data = 0;
		for (j = 0;j < 8;j++)
		{
			mask = 0x01 << i;
			data >>= 1;
			data &= 0x7f;
			if (src[j] & mask)
			{
				data |= 0x80;
			}
		}
		dest[7 - i] = ~data;
	}
}
void convBMP_12864(
    uint8_t bufpic[HEIGHT_LCD][WIDTH_LCD / 8],
    uint8_t buf12864[HEIGHT_LCD / 8][WIDTH_LCD])
{
	int iw,ih,i;
	uint8_t bufs[8], bufd[8];

	for(ih = 0; ih < (HEIGHT_LCD / 8); ih++)
	{
		for(iw = 0; iw < (WIDTH_LCD / 8); iw++)
		{
			//Get source
			for(i = 0; i < 8; i++)
			{
				bufs[i] = bufpic[ih * 8 + i][iw];
				bufd[i] = 0;
			}

			//Process data
			_rotateData(bufs, bufd);

			//Write to dest
			for (i = 0; i < 8; i++)
			{
				buf12864[ih][iw * 8 + i] = bufd[i];
			}
		}
	}
}
void outHexText(uint8_t *buf12864)
{
	int i, j;
    uint8_t value;

	for(i = 0; i < (HEIGHT_LCD * WIDTH_LCD / 64); i++)
	{
        printf("11");
		for (j = 0; j < 8; j++)
		{
            value = buf12864[i * 8 + j];
            if (value >= 0x10)
            {
			    printf("%x", value);
            }
            else
            {
			    printf("0%x", value);
            }
		}
        //putchar('\n');
	}
}
