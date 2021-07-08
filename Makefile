CFLAGS := -Wall -Wextra -pedantic -std=c11
LIBS   := -lraylib
OUTPUT := gol

ifeq ($(OS),Windows_NT)
  OUTPUT := $(OUTPUT).exe
  LIBS   := $(LIBS) -lopengl32 -lgdi32 -lwinmm
  CFLAGS := $(CFLAGS) -I include/ -L lib/
endif

gol: main.c
	gcc $(CFLAGS) -o $(OUTPUT) main.c $(LIBS)
