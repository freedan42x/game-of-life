CFLAGS=-Wall -Wextra -pedantic -std=c11
LIBS=-lraylib -lopengl32 -lgdi32 -lwinmm

gol: main.c
	gcc $(CFLAGS) -o gol.exe main.c -I include/ -L lib/ $(LIBS)