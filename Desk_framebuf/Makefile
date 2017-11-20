CC=arm-linux-gcc
OBJ=main.o font_8x16.o screen_init.o touch.o dir_opt.o mplay.o lianbiao.o draw_show.o draw_ico.o

all:$(OBJ) 
	$(CC) -o $@  $^

.PHONY:clean
clean:
	rm *.o all
