#include "head.h"

unsigned int *pmem = NULL;
struct fb_var_screeninfo vinfo;
extern unsigned char fontdata_8x16[4096];

PICINFO bmp;   //图片信息结构体
BITMAPFILEHEADER bmp_head;  //图片头信息
BITMAPINFOHEADER bmp_info;	//图片其他信息 ，位图宽，高，像素位数

/***打开图片读取头信息到***/
void get_bmpinfo(char *name, PICINFO *pic)
{
	int fd = open(name, O_RDONLY);
	if (fd < 0) {
		perror("open bmp:");
	}
	read(fd, &bmp_head, sizeof(bmp_head));
	read(fd, &bmp_info, sizeof(bmp_info));
	pic->high = bmp_info.biHeight;
	pic->width = bmp_info.biWidth;
	pic->bite_pix = bmp_info.biBitCount;
	pic->bmp_point = malloc(pic->high*pic->width*pic->bite_pix/8);
	read(fd, pic->bmp_point, pic->high*pic->width*pic->bite_pix/8); // 读出所有颜色信息
	close(fd);
	printf("****fd = %d\n",fd);
}



/***指定坐标打印纯色块***/
void display_xy(int x,int y,int col)
{
	int i = 0;
	int j = 0;
	for (i = y; i < y+90; i++) {
		for (j = x; j < x+85; j++) {
			draw_point(j, i, col);
		}
	}
}

int open_dev_event()
{
	int fd = open("/dev/input/event0", O_RDONLY);
	if (fd < 0){
		perror("open input:");
	}

	return fd;
}


int main(int argc, const char *argv[])
{
	VALUE xy_value;

	int fd = open_dev_event();

	while(1)
	{
		INFO *head = create_file_link();   //创建目录链表函数
		INFO *find = head;

		/**显示目录***/
		fb_init();
		display(COL(255,255,255));
		draw_conten(head); //打印链表文件图标
		draw_ico(719,399,"/mnt/screen/filemanage/icobmp/back.bmp"); //打印返回上一级目录的图标

		show_link(head);
		//	printf("main_bmp:x[%d],y[%d]\n",bmp.sx,bmp.sy);
		get_touch(fd, &xy_value,head);
		if ((xy_value.x>710)&&(xy_value.x<800)&&(xy_value.y>390)&&(xy_value.y<480)) {
			memset(&xy_value, 0, sizeof(xy_value));
			back_pre_dir();
		}
	
		while (1)  //
		{
			if (find == NULL)
				break;
			if (2 == find->Data.flag)
			{
				if ((xy_value.x>find->Data.ico_x)&&(xy_value.x<find->Data.ico_x+90)&&(xy_value.y>find->Data.ico_y)&&(xy_value.y<find->Data.ico_y+90)) {
					memset(&xy_value, 0, sizeof(xy_value));
					into_file(find->Data.name);
					break;
				}
			}
			if (3 == find->Data.flag)
			{
				if ((xy_value.x>find->Data.ico_x)&&(xy_value.x<find->Data.ico_x+90)&&(xy_value.y>find->Data.ico_y)&&(xy_value.y<find->Data.ico_y+90)) {
					memset(&xy_value, 0, sizeof(xy_value));
					play_mp4(find->Data.name,fd,head);
					//void play_mp4(char *name,int fd,VALUE *touch_value,INFO *head)
					break;
				}
			}
			if ((xy_value.x>710)&&(xy_value.x<800)&&(xy_value.y>390)&&(xy_value.y<480)) {
				memset(&xy_value, 0, sizeof(xy_value));
				back_pre_dir();
			}
			find = find->next;
		}
	}
	close(fd);
	return 0;
}
