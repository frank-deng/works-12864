#include <stdio.h>
#include "bmp.h"
#include "help.h"

int main(int argc,char *argv[])
{
	char *pathbmp;
	uint8_t bufpic[HEIGHT_LCD][WIDTH_LCD / 8];
    uint8_t buf12864[HEIGHT_LCD / 8][WIDTH_LCD];

	//No argument given, just print usage, then exit
	if (argc < 2)
	{
        help();
		return 1;
	}

	//I only accept the FIRST argument as the BMP file to convert
	pathbmp = argv[1];

	/*
	Now go on
	BMP format MUST be 128 x 64 x 1bit
	 */
	if(!loadBMP(pathbmp, bufpic)) //Load Failed, possibly wrong format
	{
		return -1;
	}

	//Start converting
	convBMP_12864(bufpic, buf12864);

	//Output as text of C program
	outHexText(buf12864);

	//finish
	return 0;
}
