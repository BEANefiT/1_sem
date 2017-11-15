CC=gcc
CFLAGS=-I./../log/
all: stack.o
log.o: ./../log/log.c ./../log/log.h
			$(MAKE) -C ./../log
stack.o: ./stack.c ./stack.h log.o
			$(CC) -c -o ./../$@ $< $(CFLAGS)
