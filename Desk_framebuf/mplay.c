#include "head.h"

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
