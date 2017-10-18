#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *get_src( char *src_file_name, size_t *src_sz );
size_t srcSize( FILE *src );
void *compile( char* src, size_t src_sz );

int main( char *argv[] )
{
	size_t src_sz = 0;
	char *src = get_src( argv[ 1 ], &src_sz );
	void *exe = compile( src, src_sz );
	
	return 0;
}

char *get_src( char *src_file_name, size_t *src_sz )
{
	FILE *src_file = fopen( src_file_name, "r" );
	*src_sz = srcSize( src_file );
	char *src = ( char * )calloc( sizeof( char ), *src_sz );
	fread( src, sizeof( char ), *src_sz, src_file );
	fclose( src_file );
	return src;
}

size_t srcSize( FILE *src )
{
	fseek( src, 0, SEEK_END );
	size_t src_sz = ftell( src );
	rewind( src );
	return src_sz;
}

void *compile( char *src, size_t src_sz )
{
	#define DEF_CMD( name, num ) CMD_##name = num,
	enum commands
	{
		#include "commands.h"
		CMD_zero = 0
	};
	#undef DEF_CMD

	void *exe = calloc( sizeof( double ), src_sz );
	void *exe_cur = exe;	
	int src_cur = 0;
	int src_cur_delta = 0;
	while( src_cur < src_sz )
	{
		char *str = NULL;
		sscanf( src + src_cur, "%s%n", str, src_cur_delta );
		src_cur += src_cur_delta;

		if( strcmp( str, "push" ) == 0 )
		{
			memcpy( exe_cur, &CMD_push, sizeof( int ) );
			exe_cur += sizeof( int );
			double value;
			sscanf( src + src_cur, "%d%lg", value, src_cur_delta );
			src_cur += src_cur_delta;
			memcpy( exe_cur, &value, sizeof( double ) );
			exe_cur += sizeof( double );
		}
		if( strcmp( str, "add" ) == 0 )
		{
			memcpy( exe_cur, &CMD_add, sizeof( int ) );
			exe_cur += sizeof( int );
		}
		if( strcmp( str, "mul" ) == 0 )
		{
			memcpy( exe_cur, &CMD_mul, sizeof( int ) );
			exe_cur += sizeof( int );
		}
		if( strcmp( str, "div" ) == 0 )
		{
			memcpy( exe_cur, &CMD_div, sizeof( int ) );
			exe_cur += sizeof( int );
		}
		if( strcmp( str, "sub" ) == 0 )
		{
			memcpy( exe_cur, &CMD_sub, sizeof( int ) );
			exe_cur += sizeof( int );
		}
	}
	return exe;
}
