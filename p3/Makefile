C = gcc
CFLAGS = -Wall -std=c99 -g

spellcheck: spellcheck.o document.o wordlist.o

spellcheck.o: spellcheck.c document.h wordlist.h

document.o: document.c document.h wordlist.h

wordlist.o: wordlist.c wordlist.h

clean:
	rm -f spellcheck.o document.o wordlist.o
	rm -f spellcheck
	rm -f output.txt