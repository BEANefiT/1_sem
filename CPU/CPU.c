#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./../stack_t/stack.h"
#include "./../log/log.h"

int run( double *ax, double *bx, double *cx, double *dx );
void *getcode( size_t *exe_sz );
size_t fSize( FILE *file_name );

int main()
{
	log_constr();
	double ax = 0, bx = 0, cx = 0, dx = 0;
	run( &ax, &bx, &cx, &dx );
	log_destr();
	return 0;
}

int run( double *ax, double *bx, double *cx, double *dx )
{

	stack( double, values );
	size_t exe_sz = 0;
	void *exe = getcode( &exe_sz );
	void *exe_cur = exe;
	#define DEF_CMD( name, num ) name = num,
	enum CMD
	{
		#include "./../Compilier/commands.h"
		end = 0
	};
	#undef DEF_CMD
	while( exe_cur < exe + exe_sz )
	{

		int cmd = 0;
		memcpy( &cmd, exe_cur, sizeof( int ) );
		exe_cur += sizeof( int );
		switch( cmd )
		{
			case PUSH:
			{
				double value = 0;
				memcpy( &value, exe_cur, sizeof( double ) );
				exe_cur += sizeof( double );
				Do( push( values, &value ) );
				break;
			}

			case ADD:
			{
				double value1 = 0, value2 = 0;
				Do( pop( values, &value1 ) );
				Do( pop( values, &value2 ) );
				double result = value1 + value2;
				Do( push( values, &result ) );
				break;
			}

			case MUL:
			{
				double value1 = 0, value2 = 0;
				Do( pop( values, &value1 ) );
				Do( pop( values, &value2 ) );
				double result = value1 * value2;
				Do( push( values, &result ) );
				break;
			}

			case DIV:
			{
				double value1 = 0, value2 = 0;
				Do( pop( values, &value2 ) );
				Do( pop( values, &value1 ) );
				double result = value1 / value2;
				Do( push( values, &result ) );
				break;
			}
			
			case SUB:
			{
				double value1 = 0, value2 = 0;
				Do( pop( values, &value2 ) );
				Do( pop( values, &value1 ) );
				double result = value1 - value2;
				Do( push( values, &result ) );
				break;
			}
		}

	}

}

void *getcode( size_t *exe_sz )
{
	FILE *exe_file = fopen( "exe", "r" );
	*exe_sz = fSize( exe_file );
	void *exe = calloc( *exe_sz, sizeof( char ) );
	fread( exe, sizeof( char ), *exe_sz, exe_file );
	fclose( exe_file );
	return exe;
}

size_t fSize( FILE *file_name )
{
	fseek( file_name, 0, SEEK_END );
	size_t file_sz = ftell( file_name );
	rewind( file_name );
	return file_sz;
}
