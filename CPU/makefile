CC=gcc
LIBS=-lm
CFLAGS=-I./../log/ -I./../stack_t/ -I./../Compilier/ -I./
TARGETS=./../cpu.o ./../stack.o ./../log.o
all: cpu
log.o: ./../log/log.c ./../log/log.h
			$(MAKE) -C ./../log
stack.o: ./../stack_t/stack.c ./../stack_t/stack.h
			$(MAKE) -C ./../stack_t
cpu.o: ./CPU.c ./CPU.h ./../Compilier/commands.h
			$(CC) -c -o ./../$@ $< $(CFLAGS)
cpu: cpu.o stack.o log.o
			$(CC) -o ./../$@ $(TARGETS) $(LIBS) $(CFLAGS)
