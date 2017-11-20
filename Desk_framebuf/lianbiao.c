#include "head.h"

/**链表创建函数***/
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
