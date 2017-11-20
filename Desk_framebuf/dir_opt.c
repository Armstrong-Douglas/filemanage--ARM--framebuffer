#include "head.h"

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


