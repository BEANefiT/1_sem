#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	#define DEF_CMD( name, num ) const int CMD_##name = num;
	#include "commands.h"
	#undef DEF_CMD

	void *exe_cur = exe;
	int src_cur = 0;
	int src_cur_delta = 0;

	#define _Compile_
	#include "commands.h"

	if( mode == first )
	{
		while( src_cur < src_sz )
		{
			char *str = ( char * )calloc( 10, sizeof( char ) );
			sscanf( src + src_cur, "%s%n", str, &src_cur_delta );
			src_cur += src_cur_delta;
			if( strcmp( str, "label" ) == 0 )
			{
				CMD_LABEL_code;
			}
			if( strcmp( str, "push" ) == 0 )
			{
				char *tmp = ( char * )calloc( 8, sizeof( char ) );
				char tmpc = 0;
				sscanf( src + src_cur + 1, "%c", &tmpc );
				if( tmpc == '[' )
				{
					exe_cur += 2 * sizeof( int );
				}
				else
				{
					sscanf( src + src_cur, "%s", tmp );
					if( strcmp( tmp, "ax" ) == 0 || strcmp( tmp, "bx" ) == 0 || strcmp( tmp, "cx" ) == 0 || strcmp( tmp, "dx" ) == 0 )
					{
						exe_cur += 2 * sizeof( int );
					}
					else
					{
						exe_cur += ( sizeof( int ) + sizeof( double ) );
					}
				}
				free( tmp );
			}
			if( strcmp( str, "pop" ) == 0 )
			{
				exe_cur += 2 * sizeof( int );
			}
			if( strcmp( str, "add" ) == 0 )
			{
				exe_cur += sizeof( int );
			}
			if( strcmp( str, "mul" ) == 0 )
			{
				exe_cur += sizeof( int );
			}
			if( strcmp( str, "div" ) == 0 )
			{
				exe_cur += sizeof( int );
			}
			if( strcmp( str, "sub" ) == 0 )
			{
				exe_cur += sizeof( int );
			}
			if( strcmp( str, "out" ) == 0 )
			{
				exe_cur += sizeof( int );
			}
			if( strcmp( str, "jmp" ) == 0 )
			{
				exe_cur += sizeof( int ) + sizeof( size_t );
			}
			if( strcmp( str, "je" ) == 0 )
			{
				exe_cur += sizeof( int ) + sizeof( size_t );
			}
			if( strcmp( str, "jne" ) == 0 )
			{
				exe_cur += sizeof( int ) + sizeof( size_t );
			}
			if( strcmp( str, "ja" ) == 0 )
			{
				exe_cur += sizeof( int ) + sizeof( size_t );
			}
			if( strcmp( str, "jae" ) == 0 )
			{
				exe_cur += sizeof( int ) + sizeof( size_t );
			}
			if( strcmp( str, "jb" ) == 0 )
			{
				exe_cur += sizeof( int ) + sizeof( size_t );
			}
			if( strcmp( str, "jbe" ) == 0 )
			{
				exe_cur += sizeof( int ) + sizeof( size_t );
			}

			free( str );
		}

		return 0;
	}
	
	if( mode == second )
	{
		while( src_cur < src_sz )							
		{
			char *str = ( char * )calloc( 10, sizeof( char ) );
			sscanf( src + src_cur, "%s%n", str, &src_cur_delta );
			src_cur += src_cur_delta;
	
			if( strcmp( str, "push" ) == 0 )
			{
				char *tmp = ( char * )calloc( 8, sizeof( char ) );
				char tmpc = 0;
				sscanf( src + src_cur + 1, "%c", &tmpc );
				if( tmpc == '[' )
				{
					sscanf( src + src_cur + 1, "[%[^]]]n", tmp, &src_cur_delta );
					if( strcmp( tmp, "ax" ) == 0 || strcmp( tmp, "bx" ) == 0 || strcmp( tmp, "cx" ) == 0 || strcmp( tmp, "dx" ) == 0 )
					{
						src_cur += src_cur_delta;
						memcpy( exe_cur, &CMD_RAMPUSHR, sizeof( int ) );
						exe_cur += sizeof( int );
						int reg_num = 0;
						if( strcmp( tmp, "ax" ) == 0 )
						{
							reg_num = 1;
						}
						if( strcmp( tmp, "bx" ) == 0 )
						{
							reg_num = 2;
						}
						if( strcmp( tmp, "cx" ) == 0 )
						{
							reg_num = 3;
						}
						if( strcmp( tmp, "dx" ) == 0 )
						{
							reg_num = 4;
						}
						memcpy( exe_cur, &reg_num, sizeof( int ) );
						exe_cur += sizeof( int );
					}
					else
					{
						int index = 0;
						sscanf( src + src_cur, "[%d]%n", &index, src_cur_delta );
						src_cur += src_cur_delta;
						memcpy( exe_cur, &CMD_RAMPUSH, sizeof( int ) );
						exe_cur += sizeof( int );
						memcpy( exe_cur, &index, sizeof( int ) );
						exe_cur += sizeof( int );
					}
				}
				else
				{
					sscanf( src + src_cur, "%s", tmp );
					if( strcmp( tmp, "ax" ) == 0 || strcmp( tmp, "bx" ) == 0 || strcmp( tmp, "cx" ) == 0 || strcmp( tmp, "dx" ) == 0 )
					{
						memcpy( exe_cur, &CMD_PUSHR, sizeof( int ) );
						exe_cur += sizeof( int );
						REG_READ_code;
					}
					else
					{
						memcpy( exe_cur, &CMD_PUSH, sizeof( int ) );
						exe_cur += sizeof( int );
						CMD_PUSH_code;
					}
					free( tmp );
				}
			}
			if( strcmp( str, "pop" ) == 0 )
			{
				char *tmp = ( char * )calloc( 8, sizeof( char ) );
				char tmpc = 0;
				sscanf( src + src_cur + 1, "%c", &tmpc );
				if( tmpc == '[' )
				{
					sscanf( src + src_cur + 1, "[%[^]]]n", tmp, &src_cur_delta );
					if( strcmp( tmp, "ax" ) == 0 || strcmp( tmp, "bx" ) == 0 || strcmp( tmp, "cx" ) == 0 || strcmp( tmp, "dx" ) == 0 )
					{
						src_cur += src_cur_delta;
						memcpy( exe_cur, &CMD_RAMPOPR, sizeof( int ) );
						exe_cur += sizeof( int );
						int reg_num = 0;
						if( strcmp( tmp, "ax" ) == 0 )
						{
							reg_num = 1;
						}
						if( strcmp( tmp, "bx" ) == 0 )
						{
							reg_num = 2;
						}
						if( strcmp( tmp, "cx" ) == 0 )
						{
							reg_num = 3;
						}
						if( strcmp( tmp, "dx" ) == 0 )
						{
							reg_num = 4;
						}
						memcpy( exe_cur, &reg_num, sizeof( int ) );
						exe_cur += sizeof( int );
					}
					else
					{
						int index = 0;
						sscanf( src + src_cur, "[%d]%n", &index, src_cur_delta );
						src_cur += src_cur_delta;
						memcpy( exe_cur, &CMD_RAMPOP, sizeof( int ) );
						exe_cur += sizeof( int );
						memcpy( exe_cur, &index, sizeof( int ) );
						exe_cur += sizeof( int );
					}
				}
				else
				{
					memcpy( exe_cur, &CMD_POPR, sizeof( int ) );
					exe_cur += sizeof( int );
					REG_READ_code;
				}
			}
			if( strcmp( str, "add" ) == 0 )
			{
				memcpy( exe_cur, &CMD_ADD, sizeof( int ) );
				exe_cur += sizeof( int );
			}
			if( strcmp( str, "mul" ) == 0 )
			{
				memcpy( exe_cur, &CMD_MUL, sizeof( int ) );
				exe_cur += sizeof( int );
			}
			if( strcmp( str, "div" ) == 0 )
			{
				memcpy( exe_cur, &CMD_DIV, sizeof( int ) );
				exe_cur += sizeof( int );
			}
			if( strcmp( str, "sub" ) == 0 )
			{
				memcpy( exe_cur, &CMD_SUB, sizeof( int ) );
				exe_cur += sizeof( int );
			}
			if( strcmp( str, "out" ) == 0 )
			{
				memcpy( exe_cur, &CMD_OUT, sizeof( int ) );
				exe_cur += sizeof( int );
			}
			if( strcmp( str, "jmp" ) == 0 )
			{
				memcpy( exe_cur, &CMD_JMP, sizeof( int ) );
				exe_cur += sizeof( int );
				CMD_JMP_code;
			}
			if( strcmp( str, "je" ) == 0 )
			{
				memcpy( exe_cur, &CMD_JE, sizeof( int ) );
				exe_cur += sizeof( int );
				CMD_JMP_code;
			}
			if( strcmp( str, "jne" ) == 0 )
			{
				memcpy( exe_cur, &CMD_JNE, sizeof( int ) );
				exe_cur += sizeof( int );
				CMD_JMP_code;
			}
			if( strcmp( str, "ja" ) == 0 )
			{
				memcpy( exe_cur, &CMD_JA, sizeof( int ) );
				exe_cur += sizeof( int );
				CMD_JMP_code;
			}
			if( strcmp( str, "jae" ) == 0 )
			{
				memcpy( exe_cur, &CMD_JAE, sizeof( int ) );
				exe_cur += sizeof( int );
				CMD_JMP_code;
			}
			if( strcmp( str, "jb" ) == 0 )
			{
				memcpy( exe_cur, &CMD_JB, sizeof( int ) );
				exe_cur += sizeof( int );
				CMD_JMP_code;
			}
			if( strcmp( str, "jbe" ) == 0 )
			{
				memcpy( exe_cur, &CMD_JBE, sizeof( int ) );
				exe_cur += sizeof( int );
				CMD_JMP_code;
			}
			if( strcmp( str, "call" ) == 0 )
			{
				memcpy( exe_cur, &CMD_CALL, sizeof( int ) );
				exe_cur += sizeof( int );
				CMD_JMP_code;
			}
			if( strcmp( str, "ret" ) == 0 )
			{
				memcpy( exe_cur, &CMD_RET, sizeof( int ) );
				exe_cur += sizeof( int );
			}
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
