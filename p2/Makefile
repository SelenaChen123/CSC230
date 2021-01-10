CC = gcc
CFLAGS = -Wall -std=c99 -g

sudoku: sudoku.o solve.o validate.o

sudoku.o: sudoku.c solve.h validate.h

solve.o: solve.c solve.h validate.h

validate.o: validate.c validate.h

clean:
	rm -f sudoku.o solve.o validate.o
	rm -f sudoku
	rm -f output.txt
