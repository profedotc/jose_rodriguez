.PHONY: all clean test install 


CC = gcc

all: release

release: CFLAGS = -Wall -O2
release: gol

debug: CFLAGS = -Wall -ggdb -O0
debug: gol

gol: main.o gol.o
	$(CC) $(CFLAGS) main.o gol.o -o gol

main.o: main.c gol.h
	$(CC) $(CFLAGS) -c main.c

gol.o: gol.c gol.h
	$(CC) $(CFLAGS) -c gol.c

clean: 
	rm -f *.o gol

test:
	valgrind --leak-check=full ./gol

install: 
	install -m 755 gol /bin/gol
