CC=gcc
TARGETS=log.o stack.o tree.o compilier cpu.o cpu akinator.o Akinator dump exe
log.o: ./log/log.c ./log/log.h
			$(MAKE) -C ./log/
stack.o: ./stack_t/stack.c ./stack_t/stack.h ./log.o
			$(MAKE) -C ./stack_t/
tree.o: ./tree_t/tree.c ./tree_t/tree.h ./log.o
			$(MAKE) -C ./tree_t/
compilier: ./Compilier/Compiler.c ./Compilier/Compiler.h ./Compilier/commands.h
			$(MAKE) -C ./Compilier/
cpu.o: ./CPU/CPU.c ./CPU/CPU.h ./Compilier/commands.h
			$(MAKE) -C ./CPU/
cpu: ./cpu.o
			$(MAKE) -C ./CPU/
akinator.o: ./akinator/akinator.c ./akinator/akinator.h log.o tree.o stack.o
			$(MAKE) -C ./akinator/
Akinator: akinator.o
			$(MAKE) -C ./akinator/
.PHONY: clean
clean:
			rm -rf *.o log.txt $(TARGETS)
