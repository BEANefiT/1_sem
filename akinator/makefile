CC=gcc
CFLAGS=-I./../tree_t/ -I./../stack_t/ -I./../log/
TARGETS=./../akinator.o ./../log.o ./../stack.o ./../tree.o
all: Akinator
log.o: ./../log/log.c ./../log/log.h
				$(MAKE) -C ./../log
tree.o: ./../tree_t/tree.c ./../tree_t/tree.h ./../log.o
				$(MAKE) -C ./../tree_t
stack.o: ./../stack_t/stack.c ./../stack_t/stack.h ./../log.o
				$(MAKE) -C ./../stack_t
akinator.o: ./akinator.c ./akinator.h log.o stack.o tree.o
				$(CC) -c -o ./../$@ $< $(CFLAGS)
Akinator: akinator.o log.o stack.o tree.o
				$(CC) -o ./../$@ $(TARGETS) $(CFLAGS)
