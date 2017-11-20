#include "head.h"

/**从dev下获取触屏设备活动信息，判断触屏动作,带有背景效果***/
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
		draw_touch_backico(touch_value->x,touch_value->y,head);

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


