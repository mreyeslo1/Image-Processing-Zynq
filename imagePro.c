#include "xil_types.h"
#include "xil_cache.h"
#include "../video_demo.h"
#include "graphics.h"
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "math.h"


// sqrt_approx.c
u8 sqrt_approx(int value)
{
	if (value==1) {
		return 1;
	} else if (value>1 && value<=10) {
		return 2;
	} else if (value>10 && value<=20) {
		return 3;
	} else if (value>20 && value<=40) {
		return 5;
	}  else if (value>40 && value<=50) {
		return 6;
	}  else if (value>50 && value<=60) {
		return 7;
	}  else if (value>60 && value<=80) {
		return 8;
	}  else if (value>80 && value<=100) {
		return 9;
	}  else if (value>100 && value<=120) {
		return 10;
	}  else if (value>120 && value<=140) {
		return 11;
	}  else if (value>140 && value<=170) {
		return 12;
	}  else if (value>170 && value<=200) {
		return 13;
	}  else if (value>200 && value<=220) {
		return 14;
	}  else if (value>220 && value<=240) {
		return 15;
	}  else if (value>240) {
		return 16;
	}
	return 0;
}

//void sobel(u8 *srcFrame, u8 *destFrame, u32 width, u32 height, u32 stride)
void sobel(u8 *srcFrame, u8 *destFrame)
{
	//declare x, y coordinates
	u32 xcoi, ycoi;
	u32 width = 1280;
	u32 height = 720;
	u32 stride = 5760;
	u32 lineStart = 0;

	for(ycoi = 3; ycoi < height-3; ycoi++)
	{
		for(xcoi = 3; xcoi < ((width-3) * 3); xcoi+=3)
		{

			destFrame[xcoi + lineStart] = (u8)sqrt(
			pow(
			abs(
			2*srcFrame[xcoi + lineStart-3]
			+ srcFrame[xcoi + lineStart-3-stride]
			+ srcFrame[xcoi + lineStart-3+stride]

			- 2*srcFrame[xcoi + lineStart+3]
			- srcFrame[xcoi + lineStart+3-stride]
			- srcFrame[xcoi + lineStart+3+stride])
			,2)
			+
			pow(
			abs(
			2*srcFrame[xcoi + lineStart-stride]
			+srcFrame[xcoi + lineStart-stride-3]
			+srcFrame[xcoi + lineStart-stride+3]

			-2*srcFrame[xcoi + lineStart+stride]
			-srcFrame[xcoi + lineStart+stride-3]
			-srcFrame[xcoi + lineStart+stride+3])
			,2)
			);

		   destFrame[xcoi + (lineStart + 1)] = destFrame[xcoi + lineStart];
		   destFrame[xcoi + (lineStart + 2)] = destFrame[xcoi + lineStart];
		}
		lineStart += stride;
	}
}


void passthrough(u8 *srcFrame, u8 *destFrame, u32 width, u32 height, u32 stride)
{
	u32 xcoi, ycoi;
	u32 lineStart = 0;
	for(ycoi = 0; ycoi < height; ycoi++)
	{
		for(xcoi = 0; xcoi < (width * 3); xcoi+=3)
		{
			destFrame[xcoi + lineStart] = srcFrame[xcoi + lineStart];
			destFrame[xcoi + lineStart + 1] = srcFrame[xcoi + lineStart+1];
			destFrame[xcoi + lineStart + 2] = srcFrame[xcoi + lineStart+2];
		}
		lineStart += stride;
	}
}



void flipV(u8 *srcFrame, u8 *destFrame, u32 width, u32 height, u32 stride)
{
	u32 xcoi, ycoi;
	u32 lineStart = 0;
	for(ycoi = 0; ycoi < height; ycoi++)
	{
		for(xcoi = 0; xcoi < (width * 3); xcoi+=3)
		{
			destFrame[xcoi + lineStart] = srcFrame[height*stride + xcoi - lineStart - stride];
			destFrame[xcoi + lineStart + 1] = srcFrame[height*stride + xcoi - lineStart - stride+1];
			destFrame[xcoi + lineStart + 2] = srcFrame[height*stride + xcoi - lineStart - stride+2];
		}
		lineStart += stride;
	}
}


void flipH(u8 *srcFrame, u8 *destFrame, u32 width, u32 height, u32 stride)
{
	u32 xcoi, ycoi;
	u32 lineStart = 0;
	for(ycoi = 0; ycoi < height; ycoi++)
	{
		for(xcoi = 0; xcoi < (width * 3); xcoi+=3)
		{
			destFrame[xcoi + lineStart] = srcFrame[lineStart+stride-xcoi-3];
			destFrame[xcoi + lineStart + 1] = srcFrame[lineStart+stride-xcoi+4];
			destFrame[xcoi + lineStart + 2] = srcFrame[lineStart+stride-xcoi+5];
		}
		lineStart += stride;
	}
}


// doesn't work
void flip(u8 *srcFrame, u8 *destFrame, u32 width, u32 height, u32 stride)
{
	u32 xcoi, ycoi;
	u32 lineStart = 0;
	for(ycoi = 0; ycoi < height; ycoi++)
	{
		for(xcoi = 0; xcoi < (width * 3); xcoi+=3)
		{
			destFrame[xcoi + lineStart] = srcFrame[height*stride - xcoi - lineStart];
			destFrame[xcoi + lineStart + 1] = srcFrame[height*stride - xcoi - lineStart+1];
			destFrame[xcoi + lineStart + 2] = srcFrame[height*stride - xcoi - lineStart+2];
		}
		lineStart += stride;
	}
}

void invertFrame(u8 *srcFrame, u8 *destFrame, u32 width, u32 height, u32 stride)
{
	u32 xcoi, ycoi;
	u32 lineStart = 0;
	for(ycoi = 0; ycoi < height; ycoi++)
	{
		for(xcoi = 0; xcoi < (width * 3); xcoi+=3)
		{
			destFrame[xcoi + lineStart] = ~srcFrame[xcoi + lineStart];         //Red
			destFrame[xcoi + lineStart + 1] = ~srcFrame[xcoi + lineStart + 1]; //Blue
			destFrame[xcoi + lineStart + 2] = ~srcFrame[xcoi + lineStart + 2]; //Green
		}
		lineStart += stride;
	}
}

void harrisCorner_new(u8 *srcFrame, u8 *destFrame,  u8 *blur, u8 *dx, u8 *dy)
{

	u32 width=1280;
	u32 height=720;
	u32 stride=1920*3;


		u32 xcoi, ycoi;
		u32 lineStart = 0;
		for(ycoi = 3; ycoi < height-3; ycoi++)
		{
			for(xcoi = 0; xcoi < (width*3); xcoi+=1)
			{
				// this line may break things. if so, replace with : blur[xcoi + lineStart] = srcFrame[xcoi*3+linestart]

				blur[xcoi + lineStart] = srcFrame[xcoi+lineStart];

			}
			lineStart += stride;
		}

		lineStart = 3*3*1920;
		for(ycoi = 3; ycoi < height-3; ycoi++)
		{
			for(xcoi = 3; xcoi < (1920*3)-9; xcoi+=3)
			{
				dx[xcoi + lineStart] = abs(blur[xcoi + lineStart-stride-1] - blur[xcoi + lineStart-stride+1] + blur[xcoi + lineStart-1] - blur[xcoi + lineStart+1] + blur[xcoi + lineStart+stride-1] - blur[xcoi + lineStart+stride+1]);
			}
			lineStart += 1920*3;
		}
		lineStart = 3*3*1920;
		for(ycoi = 3; ycoi < height-3; ycoi++)
		{
			for(xcoi = 3; xcoi < (1920*3)-9; xcoi+=3)
			{
				dy[xcoi + lineStart] = abs(blur[xcoi + lineStart-stride-1] - blur[xcoi + lineStart+stride-1] + blur[xcoi + lineStart-stride] - blur[xcoi + lineStart+stride] + blur[xcoi + lineStart-stride+1] - blur[xcoi + lineStart-stride+1]);
			}
			lineStart += 1920*3;
		}

		lineStart = 0;
		for(ycoi = 0; ycoi < height; ycoi++)
		{
			for(xcoi = 0; xcoi < (1920*3); xcoi+=3)
			{
				if (dy[xcoi + lineStart] < 165 && dx[xcoi + lineStart] < 165) {
					destFrame[xcoi + lineStart] = 0;
					destFrame[xcoi + lineStart + 1] = 0;
					destFrame[xcoi + lineStart + 2] = 0;
				} else {
					destFrame[xcoi + lineStart] = 255;
					destFrame[xcoi + lineStart + 1] = 255;
					destFrame[xcoi + lineStart + 2] = 255;
				}
			}
			lineStart += 1920*3;
		}
	}


