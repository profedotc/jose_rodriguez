.PHONY: all test clean test install 


CC = gcc

all: release

release: CFLAGS = -Wall -O2
release: gol

debug: CFLAGS = -Wall -ggdb -O0
debug: gol

test: CFLAGS = -Wall -O0
test: mem_test

gol: main.o gol.o
	$(CC) $(CFLAGS) main.o gol.o -o gol

main.o: main.c gol.h
	$(CC) $(CFLAGS) -c main.c

gol.o: gol.c gol.h
	$(CC) $(CFLAGS) -c gol.c

mem_test: mem_test.o gol.o
	$(CC) $(CFLAGS) mem_test.o gol.o -o mem_test

mem_test.o: mem_test.c gol.h
	$(CC) $(CFLAGS) -c mem_test.c

test:
	valgrind --leak-check=full ./mem_test

clean: 
	rm -f *.o gol mem_test

install: 
	install -m 755 gol /bin/gol
