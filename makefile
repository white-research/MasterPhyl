P=dh
OBJECTS=tree.o nw.o
CFLAGS = -g -Wall -O3
LDLIBS=
CC=c99

$(P): $(OBJECTS)
	$(CC) -o $(P) $(OBJECTS) $(CFLAGS)
