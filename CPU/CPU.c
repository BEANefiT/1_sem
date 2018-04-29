#define _CPU_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stack.h>
#include <log.h>
#include <CPU.h>
#include <commands.h>

#define RAM_SZ 1028 //one more define of RAM_SZ in lex_analyser.h

int main()
{
	log_constr();
	struct CPU_structure CPU;
	CPU_construct( &CPU );
	run( &CPU );
	log_destr();

	return 0;
}

int CPU_construct( struct CPU_structure *CPU )
{
	stack( /*double*/int, values );
	stack( size_t, rets );
	( CPU ) -> values = values;
	( CPU ) -> rets = rets;
	( CPU ) -> registers = ( int * )calloc( 4, sizeof( int ) ); //( double * )calloc( 4, sizeof( double ) );
	( CPU ) -> RAM = ( int * )calloc( RAM_SZ, sizeof (int ) );  //( double * )calloc( RAM_SZ, sizeof( double ) );
	( CPU ) -> RAM_sz = RAM_SZ;
	( CPU ) -> exe_sz = 0;
	( CPU ) -> exe = getcode( CPU );
}

int run( struct CPU_structure *CPU )
{
	while( ( CPU ) -> exe_cur < ( CPU ) -> exe + ( CPU ) -> exe_sz )
	{
		int cmd = 0;
		from_exe( &cmd, int );
		switch( cmd )
		{

			#define DEF_CMD( NAME, name, num, Cmplr_code2, Cmplr_code1, elf, CPU_code );	\
				case num:							\
				{								\
					CPU_code;						\
					break;							\
				}								\

			#include <commands.h>
			#undef DEF_CMD
		}
	}
}

void *getcode( struct CPU_structure *CPU )
{
	FILE *exe_file = fopen( "exe", "r" );
	( CPU ) -> exe_sz = fSize( exe_file );
	void *exe = calloc( ( CPU ) -> exe_sz, sizeof( char ) );
	fread( exe, sizeof( char ), ( CPU ) -> exe_sz, exe_file );
	fclose( exe_file );
	( CPU ) -> exe_cur = exe;
	return exe;
}

size_t fSize( FILE *file_name )
{
	fseek( file_name, 0, SEEK_END );
	size_t file_sz = ftell( file_name );
	rewind( file_name );
	return file_sz;
}
