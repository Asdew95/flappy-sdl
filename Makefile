CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99
LDFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf

DEPS=bird.h flappy.h pipe.h ui.h
OBJ=bird.o main.o pipe.o ui.o
OUT=flappy

.PHONY: clean

all: $(OUT)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUT): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(OUT)
