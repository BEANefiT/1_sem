#define _CPU_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "./../stack_t/stack.h"
#include "./../log/log.h"
#include "CPU.h"
#include "./../Compilier/commands.h"

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
	stack( double, values );
	stack( size_t, rets );
	( CPU ) -> values = values;
	( CPU ) -> rets = rets;
	( CPU ) -> RAM = ( double * )calloc( 64, sizeof( double ) );
	( CPU ) -> RAM_sz = 64;
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

			#define DEF_CMD( NAME, name, num, Cmplr_code2, Cmplr_code1, CPU_code );	\
				case num:							\
				{								\
					CPU_code;						\
					break;							\
				}								\

			#include "./../Compilier/commands.h"
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
