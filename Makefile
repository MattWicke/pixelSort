CC = g++
CFLAGS = -g
LDFLAGS = `pkg-config --libs opencv`

OBJS = main.cpp pixelSort.cpp

OBJ_NAME = srt

all : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(OBJ_NAME) 
