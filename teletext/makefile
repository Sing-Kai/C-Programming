CFLAGS =  -Wall -Wextra -Wfloat-equal -pedantic -ansi -O2 $(shell pkg-config sdl2 --cflags)
INCS = html.h textngraphics.h test.h
TARGET = teletext
SOURCES =  html.c test.c graphics.c $(TARGET).c
LIBS =  -lm $(shell pkg-config sdl2 --libs)
CC = gcc


all: $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET) 
