CC = gcc
CFLAGS = -Wall -Wextra -std=c11

mycompress: main.c
	$(CC) $(CFLAGS) main.c -o mycompress

clean:
	del mycompress.exe