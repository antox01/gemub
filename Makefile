CC=gcc

PATH_INCLUDE=/opt/homebrew/include
PATH_LIB=/opt/homebrew/lib

HEADERS_PATH=./includes

IFLAGS=-I$(PATH_INCLUDE) -I$(HEADERS_PATH)
CFLAGS=-L$(PATH_LIB) -lSDL2

OUT_FILES = out/cpu.o out/rom.o out/memory.o

gemub: setup $(OUT_FILES)
	$(CC) -g -fsanitize=address -o out/gemub src/main.c $(OUT_FILES) $(IFLAGS) $(CFLAGS)

out/cpu.o: ./includes/cpu.h ./src/cpu.c
	$(CC) -g -fsanitize=address -c -o out/cpu.o src/cpu.c

out/rom.o: ./includes/rom.h ./src/rom.c
	$(CC) -g -fsanitize=address -c -o out/rom.o src/rom.c

out/memory.o: ./includes/memory.h ./src/memory.c
	$(CC) -g -fsanitize=address -c -o out/memory.o src/memory.c

setup:
	[ -d "./out" ] || mkdir out
