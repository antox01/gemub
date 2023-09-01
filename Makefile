CC=gcc

PATH_INCLUDE=/opt/homebrew/include
PATH_LIB=/opt/homebrew/lib

HEADERS_PATH=./includes

IFLAGS=-I$(PATH_INCLUDE) -I$(HEADERS_PATH)
CFLAGS=-L$(PATH_LIB) -lSDL2

OUT_FILES = out/cpu.o out/rom.o

gemub: setup out/cpu.o out/rom.o
	$(CC) -g -o out/gemub src/main.c $(OUT_FILES) $(IFLAGS) $(CFLAGS)

out/cpu.o: ./includes/cpu.h ./src/cpu.c
	$(CC) -g -c -o out/cpu.o src/cpu.c

out/rom.o: ./includes/rom.h ./src/rom.c
	$(CC) -g -c -o out/rom.o src/rom.c

setup:
	[ -d "./out" ] || mkdir out
