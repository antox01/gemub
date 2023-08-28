CC=gcc


gemub: setup
	gcc -o out/gemub src/main.c

setup:
	[ -d "./out" ] || mkdir out
