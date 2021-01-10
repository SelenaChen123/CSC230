CC = gcc
CFLAGS = -Wall -std=c99 -g

ugrep: ugrep.o parse.o pattern.o

ugrep.o: ugrep.c parse.h pattern.h

parse.o: parse.c parse.h pattern.h

pattern.o: pattern.c pattern.h

clean:
	rm -f ugrep.o parse.o pattern.o
	rm -f ugrep
	rm -f output.txt
