CC=gcc
CFLAGS=-I./../log/ -I./

./../asm_compiler: ./Compiler.c ./Compiler.h ./commands.h
	@$(CC) -o $@ ./Compiler.c $(CFLAGS)
