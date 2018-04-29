struct CPU_structure
{
	stack_t *values;
	stack_t *rets;
	/*double*/int *RAM;
	/*double*/int *registers;
	size_t RAM_sz;
	size_t exe_sz;
	void *exe;
	void *exe_cur;
};
int CPU_construct( struct CPU_structure *CPU );
int run( struct CPU_structure *CPU );
void *getcode( struct CPU_structure *CPU );
size_t fSize( FILE *file_name );
