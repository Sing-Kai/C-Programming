CFLAGS = `sdl2-config --cflags` -Wall -Wextra -Wfloat-equal -pedantic -ansi -O2
INCS = neillsdl2.h 
TARGET = huffsdl
SOURCES =  neillsdl2.c $(TARGET).c
LIBS =  `sdl2-config --libs`
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET) 

