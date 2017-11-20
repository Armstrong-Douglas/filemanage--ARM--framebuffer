#include "head.h"

extern unsigned char fontdata_8x16[4096];

/***打点函数**/
void draw_point(int x, int y, int col)
{
	*(pmem+x+y*vinfo.xres_virtual) = col;
}

/**显示图片函数***/
void show_bmp(PICINFO *pic)
{
	int x, y;
	unsigned char * bmp;
	unsigned char r, g, b;
	bmp = pic->bmp_point;
	for (y = pic->high-1; y >=0; y--) {
		for (x = 0; x < pic->width; x++) {
			b = *pic->bmp_point++;
			g = *pic->bmp_point++;
			r = *pic->bmp_point++;
			draw_point(x+pic->sx, y+pic->sy, COL(r,g,b));
		}
	}
	free(bmp);
}

/***ascii 码**/
void draw_ascii(int x0, int y0, int col, int col_bk, char ascii)
{
	int i, j, k;
	for (i = 0; i < 16; i++) {
		for (k = 0; k < 8; k++) {
			if (fontdata_8x16[ascii*16+i] & (0x80>>k))  //利用ascii值计算字模库偏移位置
				draw_point(x0+k, y0+i, col);
			else 
				draw_point(x0+k, y0+i, col_bk);
		}
	}
}

/**纯色显示***/
void display(int col)
{
	int x, y;
	for (y = 0; y < vinfo.yres; y++) {
		for (x = 0; x < vinfo.xres; x++) {
			draw_point(x, y, col);
		}
	}
}


