CC=gcc
CFLAGS=-I./../log/ -I./../tree_t/
CTARGETS=compiler.c LNG_recurs_desc_parser.o lex_analyser.o back_end.o ./../log.o ./../tree.o

all: compiler

./../log.o:
	$(MAKE) -C ./../log

./../tree.o:
	$(MAKE) -C ./../tree_t

lex_analyser.o: lex_analyser.c lex_analyser.h ./../log.o kwrds.h
	$(CC) -c -o $@ $< $(CFLAGS)

LNG_recurs_desc_parser.o: LNG_recurs_desc_parser.c LNG_recurs_desc_parser.h lex_analyser.o ./../log.o ./../tree.o
	$(CC) -c -o $@ $< $(CFLAGS)

back_end.o: back_end.c back_end.h lex_analyser.o
	$(CC) -c -o $@ $< $(CFLAGS)

compiler: compiler.c compiler.h lex_analyser.o LNG_recurs_desc_parser.o back_end.o ./../log.o ./../tree.o
	$(CC) $(CTARGETS) $(CFLAGS) -o $@

.PHONY: clean

clean:
	rm -rf *.o dump dmp log.txt asm compiler
