C = gcc
CFLAGS = -Wall -std=c99 -g

contest: contest.o command.o model.o

contest.o: contest.c command.h model.h

command.o: command.c command.h model.h

model.o: model.c model.h

clean:
	rm -f contest.o command.o model.o
	rm -f contest
	rm -f output.txt
