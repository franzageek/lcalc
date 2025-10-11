CC=gcc
INCLUDE=include
LIB=lib

main: *.c $(LIB)/*.c
	$(CC) $^ -o $@ -I$(INCLUDE)