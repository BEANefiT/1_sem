#define _Compilier_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"

enum Compilier_modes
{
	first,
	second
};
char *get_src( char *src_file_name, size_t *src_sz );
size_t srcSize( FILE *src );
int compile( char* src, size_t src_sz, size_t *exe_sz, enum Compilier_modes mode, size_t *labels, void *exe );
int makecode( void *exe, size_t exe_sz );

int main( int argc, char *argv[] )
{
	size_t src_sz = 0;
	char *src = get_src( argv[ 1 ], &src_sz );
	size_t exe_sz = 0;
	#define label_amount 20
	size_t labels[ label_amount ] = {};
	#undef label_amount
	void *exe = calloc( sizeof( double ), src_sz );
	enum Compilier_modes mode = first;
	compile( src, src_sz, &exe_sz, mode, labels, exe );
	mode = second;
	compile( src, src_sz, &exe_sz, mode, labels, exe );
	makecode( exe, exe_sz );

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

int compile( char *src, size_t src_sz, size_t *exe_sz, enum Compilier_modes mode, size_t *labels, void *exe )
{
	#define DEF_CMD( NAME,  name, num, Cmplr_code2, Cmplr_code1, CPU_code ) const int CMD_##NAME = num;

	#include "commands.h"

	#undef DEF_CMD

	void *exe_cur = exe;
	int src_cur = 0;
	int src_cur_delta = 0;

	if( mode == first )
	{
		while( src_cur < src_sz )
		{
			char *str = ( char * )calloc( 10, sizeof( char ) );
			from_src( %s, str );
			#define DEF_CMD( NAME, name, num, Cmplr_code2, Cmplr_code1, CPU_code )		\
			do										\
			{										\
				if( strcmp( str, #name ) == 0 )						\
				{									\
					exe_cur += Cmplr_code1;						\
				}									\
			}while( 0 )

			#include "commands.h"

			#undef DEF_CMD

			free( str );
		}
		return 0;
	}
	
	if( mode == second )
	{
		while( src_cur < src_sz )							
		{
			char *str = ( char * )calloc( 10, sizeof( char ) );
			from_src( %s, str );
			#define DEF_CMD( NAME, name, num, Cmplr_code2, Cmplr_code1,  CPU_code )	\
			do										\
			{										\
				if( strcmp( str, #name ) == 0 )						\
				{									\
					Cmplr_code2;							\
				}									\
			}while( 0 )

			#include "commands.h"

			#undef DEF_CMD

			free( str );
		}
		*exe_sz = ( size_t ) exe_cur - ( size_t ) exe;
		return 0;
	}
}

int makecode( void *exe, size_t exe_sz )
{
	FILE *exe_file = fopen( "./../CPU/exe", "w" );
	fwrite( exe, exe_sz, sizeof( char ), exe_file );
	fclose( exe_file );
}
