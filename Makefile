.PHONY: all clean test install


CC = gcc
CFGLAS = -Wall

all: release

release: gol

debug: CFLAGS += -g
debug: gol

gol: main.o gol.o
	$(CC) $(CFLAGS) main.o gol.o -o gol

main.o: main.c gol.h
	$(CC) $(CFLAGS) -c main.c

gol.o: gol.c
	$(CC) $(CFLAGS) -c gol.c

clean: 
	rm -f *.o gol

test:
	./gol

install: 
	install -m 755 gol /bin/gol
