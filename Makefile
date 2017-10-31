CC=gcc
LIBS=-lm
TARGETS=compilier cpu exe log.txt
CFLAGS=-I./log/ -I./stack_t/ -I./Compilier/ -I./CPU/
log.o: ./log/log.c ./log/log.h
				$(CC) -c -o $@ $<
stack.o: ./stack_t/stack.c ./stack_t/stack.h log.o
				$(CC) -c -o $@ $<
cpu.o: ./CPU/CPU.c ./CPU/CPU.h ./Compilier/commands.h
				$(CC) -c -o $@ $< $(CFLAGS)
compilier: ./Compilier/Compiler.c ./Compilier/Compiler.h ./Compilier/commands.h
				$(CC) -o $@ $< $(CFLAGS)
cpu: cpu.o stack.o log.o
				$(CC) -o $@ $^ $(LIBS) $(CFLAGS)
.PHONY: clean
clean:
				rm -f *.o $(TARGETS)
