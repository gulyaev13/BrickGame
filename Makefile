CC=gcc
TARGET=build/BrickGame
CK= -c
LDK= -o
CFLAGS= -std=c89 -Wall -Werror -pedantic
LFLAGS= -lSDL2 -lSDL2_ttf
CFILES= src/*.c
OFILES= *.o

all:
	$(CC) $(CFLAGS) $(CK) $(CFILES)
	(ls build 1>/dev/null 2>&1) || mkdir build
	$(CC) $(LDK) $(TARGET) $(OFILES) $(LFLAGS)
	rm -f $(OFILES)
clean:
	rm -f $(TARGET)