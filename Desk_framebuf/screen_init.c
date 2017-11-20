#include "head.h"

/****屏幕初始化，获得屏幕参数***/
void fb_init(void) 
{
	int screen_size = 0;
	int fd = open("/dev/fb0", O_RDWR);
	if (fd < 0) {
		perror("open:");
	}
	memset(&vinfo, 0, sizeof(vinfo));
	//	读取屏幕尺寸信息，像素点信号
	//	FBIOGET_VSCREENINFO 得到显示参数的cmd
	ioctl(fd, FBIOGET_VSCREENINFO,&vinfo);//内核返回显示参数信息到vinfo

	screen_size = vinfo.xres_virtual*vinfo.yres_virtual*vinfo.bits_per_pixel/8;
#if 1
	pmem = mmap(NULL, screen_size,PROT_WRITE|PROT_READ, MAP_SHARED, fd,0);
	if (NULL == pmem) {
		perror("mmap");
	}
#endif 
}
