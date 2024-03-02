default: main.c
	gcc main.c -o synfo

static: main.c
	musl-gcc -s main.c -o synfo -static