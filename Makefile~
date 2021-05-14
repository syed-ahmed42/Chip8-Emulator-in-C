CC=gcc

CFLAGS=-c -std=c99 -Wall -Wextra -g -O2

SDL_CFLAGS= $(shell sdl2-config --cflags)
SDL_LFLAGS= $(shell sdl2-config --libs)

override CFLAGS += $(SDL_CFLAGS)

HEADERDIR= src/
SOURCEDIR= src/

HEADERFILES= chip8.h chip8_t.h instructions.h screen.h
SOURCEFILES= main.c chip8.c instructions.c screen.c

HEADER_FP= $(addprefix $(HEADERDIR),$(HEADERFILES))
SOURCE_FP= $(addprefix $(SOURCEDIR),$(SOURCEFILES))

OBJECTS= $(SOURCE_FP:.c = .o)
EXECUTABLE=chip8

#-------------------------------------------

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(SDL_LFLAGS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf src/*.o $(EXECUTABLE)
