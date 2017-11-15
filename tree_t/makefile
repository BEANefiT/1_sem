CC=gcc
CFLAGS=-I./../log/
all: tree.o
log.o: ./../log/log.c ./../log/log.h
			$(MAKE) -C ./../log
tree.o: ./tree.c ./tree.h log.o
			$(CC) -c -o ./../$@ $< $(CFLAGS)
