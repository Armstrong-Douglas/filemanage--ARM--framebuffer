CC=arm-linux-gcc
OBJ=main.o font_8x16.o

all:$(OBJ) 
	$(CC) -o $@  $^

.PHONY:clean
clean:
	rm *.o all
