CC=gcc
CFLAGS=-I./../log/

all: lex_analyser.o

log.o: ./../log/log.c ./../log/log.h
	$(MAKE) -C ./../log

lex_analyser.o: ./lex_analyser.c ./lex_analyser.h log.o
	$(CC) -c -o ./../$@ $< $(CFLAGS)
