CC=gcc
CC_FLAGS=-Wall -Wextra -Werror -pedantic
INCLUDE=include
LIB=lib

main: *.c $(LIB)/*.c src/*.c
	$(CC) $^ -o $@ -I$(INCLUDE) $(CC_FLAGS)

clean:
	rm main