CC=gcc
TARGET=build/BrickGame
CK= -c
LDK= -o
CFLAGS= -std=c89 -Wall -Werror -pedantic
LFLAGS= -lSDL2 -lSDL2_ttf
CFILES= src/*.c
OFILES= *.o

all: move_src mkdir_build
	$(CC) $(CFLAGS) $(CK) $(CFILES)
	$(CC) $(LDK) $(TARGET) $(OFILES) $(LFLAGS)
	rm -f $(OFILES)
move_src:
	(ls src 1>/dev/null 2>&1) || (mkdir src && mv *.[ch] src/)
mkdir_build:
	(ls build 1>/dev/null 2>&1) || mkdir build
clean:
	rm -f $(TARGET)