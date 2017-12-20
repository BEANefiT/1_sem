CC=gcc
CFLAGS=-I./../tree_t/ -I./../log/
TARGETS=./../diff.o ./../log.o ./../tree.o
all: Diff
log.o: ./../log/log.c ./../log/log.h
				$(MAKE) -C ./../log
tree.o: ./../tree_t/tree.c ./../tree_t/tree.h ./../log.o
				$(MAKE) -C ./../tree_t
diff.o: ./diff.c ./diff.h log.o tree.o
				$(CC) -c -o ./../$@ $< $(CFLAGS)
Diff: diff.o log.o tree.o
				$(CC) -o ./../$@ $(TARGETS) $(CFLAGS)
