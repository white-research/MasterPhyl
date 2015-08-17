P=dh
OBJECTS=matrix.o tree.o rf.o dynamic_homology.o output.o analysis.o main.o
CFLAGS=-g -Wall -O3 -std=c99
LDLIBS=
CC=gcc
DEPS=matrix.h tree.h rf.h dynamic_homology.h output.h analysis.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(P): $(OBJECTS)
	$(CC) -o $(P) $(OBJECTS) $(CFLAGS)


LIBDH_SOURCE=matrix.c tree.c rf.c dynamic_homology.c output.c analysis.c
LIBDH_P=libdh.so
$(LIBDH_P): $(LIBDH_OBJECTS)
	$(CC) -o $(LIBDH_P) -fPIC -shared $(LIBDH_SOURCE) $(CFLAGS)


