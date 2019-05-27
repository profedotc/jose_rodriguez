.PHONY: all clean test install

all: gol

gol: main.o gol.o
	gcc main.o gol.o -o gol

main.o: main.c gol.h
	gcc -c main.c

gol.o: gol.c
	gcc -c gol.c

clean: 
	rm -f *.o gol

test:
	./gol

install: 
	install -m 755 gol /bin/gol
