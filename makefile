CC=gcc
TARGETS=asm_compiler compiler cpu akinator diff dump dmp asm log.txt

all: compiler

log.o:
	@$(MAKE) -C ./log/

stack.o:
	@$(MAKE) -C ./stack_t/

tree.o:
	@$(MAKE) -C ./tree_t/

asm_compiler:
	@$(MAKE) -C ./Compilier/

cpu:
	@$(MAKE) -C ./CPU/

akinator:
	@$(MAKE) -C ./Akinator/

diff:
	@$(MAKE) -C ./Diff/

compiler:
	@$(MAKE) -C ./language/

.PHONY: clean
clean:
	@rm -f *.o $(TARGETS)
