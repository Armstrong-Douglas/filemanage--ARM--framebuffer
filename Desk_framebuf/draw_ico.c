#include "head.h"

PICINFO bmp;   //图片信息结构体


/***打印图标***/
void draw_ico(int x,int y,char *filename)
{
	bmp.sx = x;
	bmp.sy = y;
	get_bmpinfo(filename,&bmp);
	show_bmp(&bmp);
	printf("draw ico after show\n");
}

/**打印图标的名字***
 ***(int x0, int y0, int col, int col_bk, char ascii)**/
void prin_name(int x,int y,char *name)
{
	int x0 = x;
	int y0 = y + 74;
	char namebuf[64] = {0};
	strcpy(namebuf,name);
	int i = 0;
	while(namebuf[i] != '\0')
	{
		draw_ascii(x0+i*8,y0,COL(128,64,0),COL(255,255,255),namebuf[i]);
		i++;
	}
}

/**目录图标显示***/
void draw_conten(INFO *phead)
{	
	INFO *find = phead;
	int num_x = 0;  //记录每一行的图标个数 最多7个
	int x = 20;
	int y = 20;	

	while (find != NULL)	
	{
		if (num_x == 7) {
			num_x = 0;
			x = 10;
			y += 95;
		}

		if (1 == find->Data.flag)
		{
			draw_ico(x,y,"/mnt/screen/filemanage/icobmp/txt.bmp");
			prin_name(x,y,find->Data.name);
			x += 100;
			num_x++;
		}
		
		if (2 == find->Data.flag)
		{
			draw_ico(x,y,"/mnt/screen/filemanage/icobmp/file.bmp");
			prin_name(x,y,find->Data.name);
			find->Data.ico_x = x;
			find->Data.ico_y = y;
			printf(" x= %d y= %d\t\n",x,y);
			printf(" ico_x= %d ico_y= %d\t\n",find->Data.ico_x,find->Data.ico_y);
			x += 100;
			num_x++;
		}
		
		if (3 == find->Data.flag)
		{
			draw_ico(x,y,"/mnt/screen/filemanage/icobmp/MP4.bmp");
			prin_name(x,y,find->Data.name);
			find->Data.ico_x = x;
			find->Data.ico_y = y;
			printf(" x= %d y= %d\t\n",x,y);
			printf(" ico_x= %d ico_y= %d\t\n",find->Data.ico_x,find->Data.ico_y);
			x += 100;
			num_x++;
		}
		find = find->next;
	}
}

/***显示按下屏幕带背景图标***/
void draw_touch_backico(int x,int y,INFO *find)
{
#if 1
	while(find != NULL) {
	if ((x>find->Data.ico_x)&&(x<find->Data.ico_x+90)&&(y>find->Data.ico_y)&&(y<find->Data.ico_y+90)){
		if (find->Data.flag == 2 || find->Data.flag == 3) {
		x = find->Data.ico_x;
		y = find->Data.ico_y;
		printf("touch_back:x = %d,y = %d\n",x,y);
		display_xy(x,y,COL(109,109,109));
		draw_ico(x,y,"/mnt/screen/filemanage/icobmp/file.bmp");
		break;
		}
	}
		find = find->next;	
	}
#endif
}

