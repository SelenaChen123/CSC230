CC = gcc
CFLAGS = -Wall -std=c99 -g

all: conceal extract

conceal: conceal.o bits.o image.o

extract: extract.o bits.o image.o

conceal.o: conceal.c bits.h image.h

extract.o: extract.c bits.h image.h

bits.o: bits.c

image.o: image.c bits.h

conceal.c: bits.h image.h

extract.c: bits.h image.h

bits.c: bits.h

iamge.c: image.h bits.h

clean:
	rm -f conceal.o extract.o
	rm -f conceal extract
	rm -f output.ppm
	rm -f output.txt
	rm -f expected.txt	
