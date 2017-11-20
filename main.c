#include "head.h"



unsigned int *pmem = NULL;
struct fb_var_screeninfo vinfo;
extern unsigned char fontdata_8x16[4096];

PICINFO bmp;   //图片信息结构体
BITMAPFILEHEADER bmp_head;  //图片头信息
BITMAPINFOHEADER bmp_info;	//图片其他信息 ，位图宽，高，像素位数

/***打点函数**/
void draw_point(int x, int y, int col)
{
		*(pmem+x+y*vinfo.xres_virtual) = col;
}

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


INFO *create(INFO* phead,char *name,int flag)
{		
	INFO *temp = malloc(sizeof(INFO));
	strcpy(temp->Data.name,name);
	temp->Data.flag = flag;
	temp->Data.ico_x = 0;
	temp->Data.ico_y = 0;
	temp->next = NULL;
	
	INFO *find = phead;

	if (NULL == phead)
		return temp;
	else{ 
		while (find->next != NULL)
			find = find->next;

		find->next = temp;
	}
	return phead;
}

/***调试打印链表***/
void show_link(INFO *phead)
{
	INFO *find = phead;
	
	int i = 0;
	while (find != NULL)
	{
		printf("%s[%d]x[%d]y[%d]\t",find->Data.name,find->Data.flag,find->Data.ico_x,find->Data.ico_y);
		find = find->next;
		if (i == 5)
			printf("\n");
		i++;
	}
	printf("\n");
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

/***显示按下屏幕带背景图标***/
void draw_touch_back(int x,int y,INFO *find)
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


/**从dev下获取触屏设备活动信息，判断触屏动作***/
int get_touch(int fd, VALUE *touch_value,INFO *head)
{
	int num;
	struct input_event ev;
	int cnt = 0;
	while (1) {
		read(fd, &ev, sizeof(ev));
		if (EV_ABS == ev.type) {
			if (0x35 == ev.code) {
				touch_value->x = ev.value;
				num = 0;
				touch_value->flag = 0;
				printf("ev.x :%d\n",ev.value);
			}
			if (0x36 == ev.code) {
				touch_value->y = ev.value;
				printf("ev.y :%d\n",ev.value);
			}
		}
		if (6 == num) {
			if (0X3 == ev.type) {
				num = 0;
			}
			else {
				touch_value->flag = 1;
				break;
			}
		}
		if (touch_value->x>710&&touch_value->y>390){
			cnt++;
			num++;
			continue;
		}
		else if (cnt >1)
		draw_touch_back(touch_value->x,touch_value->y,head);

		cnt++;
		num++;
	}
	return 0;
}

/**从dev下获取触屏设备活动信息，判断触屏动作***/
int get_touch2(int fd, VALUE *touch_value,INFO *head)
{
	int num;
	struct input_event ev;
	while (1) {
		read(fd, &ev, sizeof(ev));
		if (EV_ABS == ev.type) {
			if (0x35 == ev.code) {
				touch_value->x = ev.value;
				num = 0;
				touch_value->flag = 0;
				printf("ev.x :%d\n",ev.value);
			}
			if (0x36 == ev.code) {
				touch_value->y = ev.value;
				printf("ev.y :%d\n",ev.value);
			}
		}
		if (6 == num) {
			if (0X3 == ev.type) {
				num = 0;
			}
			else {
				touch_value->flag = 1;
				break;
			}
		}
		num++;
	}
	return 0;
}
/***创建目录文件链表，区分文件类型***/
INFO* create_file_link()
{
	DIR *dir = opendir("./");
	if (NULL == dir)
	{
		perror("fail open rootdir /\n");
		return NULL;
	}
	
	struct dirent *info_d;
	INFO *head = NULL;

	while (NULL != (info_d = readdir(dir)))
	{
		char namebuf[256] = {0};
		strcpy(namebuf,info_d->d_name);
	
		//printf("name[%s]\n",namebuf);
		/**将.和..还有.开头的隐藏文件跳过*/
		if ('.' == namebuf[0])
			continue;

		char *p = NULL;
		p = strtok(namebuf,".");
		p = strtok(NULL,"\0");
		
		//printf("p[%s]\n",p);
		
		if (p != NULL &&0 == strcmp(p,"mp4")) {
			head = create(head,info_d->d_name,3);
		}
		else if (DT_REG == info_d->d_type) {
				head = create(head,info_d->d_name,1);
			}
			else if (DT_DIR == info_d->d_type) {
				head = create(head,info_d->d_name,2);
			}
#if 0	
	/**区分.h和.c文件*/
		char *p = namebuf;
		while (*p != '\0')
		{
			if (*p == '.') {
				p++;
				if (*p == 'c')
					head = create(head,info_d->d_name,10);
				else if (*p == 'h')
					head = create(head,info_d->d_name,11);
			}
			p++;
		}
#endif
	}
	return head;
}


/**返回上一级目录**/
void back_pre_dir()
{
	char buf[100] = {0};
	chdir("../");
	getcwd(buf,100);
	printf("%s\n",buf);
}

/**进入目录函数****/
void into_file(char *name)
{
	char path[1024] = {0};
	char buf[] = "./";
	sprintf(path,"%s%s",buf,name);
	printf("%s\n",path);
	chdir(path);
}

void full_back(int sig_num)
{
	system("killall mplayer");
}


/******MP4播放******/
int play_mp4(char *name,int fd,INFO *head)
{
	VALUE xy_value;	
	int ret = mkfifo("myfifo",0666);
	if (-1 == ret)
	{
		if (errno != EEXIST)
		{
			perror("fail mkfifo");
			exit(1);
		}
	}
		display(COL(0,0,0));
		draw_ico(50,400,"/mnt/screen/filemanage/icobmp/start.bmp");
		draw_ico(300,400,"/mnt/screen/filemanage/icobmp/pause.bmp");		
		draw_ico(750,10,"/mnt/screen/filemanage/icobmp/stop.bmp");
	
		pid_t pid = fork(); 
//(int fd, VALUE *touch_value,INFO *head)
	if (0 < pid) {
		while (1)
		{
			get_touch2(fd,&xy_value,head);
			if ((xy_value.x>745)&&(xy_value.x<785)&&(xy_value.y>10)&&(xy_value.y<50)) {
			/*	int send_fifo = open("myfifo",O_WRONLY);
				char buf[] = "quit\n";
				write(send_fifo,buf,sizeof(buf));
				close(send_fifo);*/
				system("killall mplayer");
				break;
			}
		/*	if ((xy_value.x>50)&&(xy_value.x<130)&&(xy_value.y>400)&&(xy_value.y<480)) {
				
			}*/
			else if ((xy_value.x>300)&&(xy_value.x<380)&&(xy_value.y>400)&&(xy_value.y<480)) {
			printf("x : %d ,y : %d\n",xy_value.x,xy_value.y);
				int send_fifo = open("myfifo",O_WRONLY);
				char buff[] = "pause\n";
				write(send_fifo,buff,6);
				close(send_fifo);
			}
		}
			signal(SIGCHLD,SIG_IGN);
	}
	if (0 == pid)
	{
		execl("/bin/mplayer","mplayer","-slave","-input","file=/mnt/screen/filemanage/myfifo",name,NULL);
	}
/* 	
	if (0 == pid)
	{
		pid_t pid = fork();
		if (0 < pid){
			wait(NULL);
		}
		if (0 == pid){
			execl("/bin/mplayer","mplayer","-slave","-input","file=/mnt/screen/filemanage/myfifo",name,NULL);
		}
	}*/
	
	
}

int open_frambuf_touch()
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

	int fd = open_frambuf_touch();

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
