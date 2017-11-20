#ifndef __HEAD_H
#define __HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <math.h>
#include <linux/input.h>
#include <errno.h>
#include <sys/wait.h>
#define COL(r,g,b) ((r<<16)|(g<<8)|(b))
extern unsigned int *pmem;
extern struct fb_var_screeninfo vinfo;

typedef struct Data_dir {
	char name[256];
	int  flag;
	int ico_x;   //为触屏进入文件夹传入一个起始点
	int ico_y;
	
}DATA;  

typedef struct Info_pass {
	DATA Data;
	struct Info_pass *next;
}INFO;



typedef struct tagBITMAPFILEHEADER
{
	short bfType;//位图文件的类型，必须为BM(1-2字节)
	int bfSize;//位图文件的大小，以字节为单位（3-6字节，低位在前）
	short bfReserved1;//位图文件保留字，必须为0(7-8字节)
	short bfReserved2;//位图文件保留字，必须为0(9-10字节)
	int bfOffBits;//位图数据的起始位置，以相对于位图（11-14字节，低位在前）
	//文件头的偏移量表示，以字节为单位
} __attribute__((packed)) BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
	int biSize;//本结构所占用字节数（15-18字节）
	long biWidth;//位图的宽度，以像素为单位（19-22字节）
	long biHeight;//位图的高度，以像素为单位（23-26字节）
	short biPlanes;//目标设备的级别，必须为1(27-28字节）
	short biBitCount;//每个像素所需的位数，必须是1（双色），（29-30字节） 4(16色），8(256色）16(高彩色)或24（真彩色）之一
	int biCompression;//位图压缩类型，必须是0（不压缩），（31-34字节）
	//1(BI_RLE8压缩类型）或2(BI_RLE4压缩类型）之一
	int biSizeImage;//位图的大小(其中包含了为了补齐行数是4的倍数而添加的空字节)，以字节为单位（35-38字节）
	long biXPelsPerMeter;//位图水平分辨率，每米像素数（39-42字节）
	long biYPelsPerMeter;//位图垂直分辨率，每米像素数（43-46字节)
	int biClrUsed;//位图实际使用的颜色表中的颜色数（47-50字节）
	int biClrImportant;//位图显示过程中重要的颜色数（51-54字节）
} __attribute__((packed)) BITMAPINFOHEADER;

typedef struct pic_info {
	int sx;
	int sy;
	int width;
	int high;
	int bite_pix;
	unsigned char *bmp_point;
}PICINFO;  //显示使用图片信息参数


typedef struct input_value {
	int x; 
	int y; 
	int flag;
}VALUE;


#endif
