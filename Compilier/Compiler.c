#define _Compilier_
#define label_amount 128
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <commands.h>
#include <Compiler.h>

int main( int argc, char *argv[] )
{
	struct Compilier_structure Compilier;
	Compilier_constr( &Compilier );
	Compilier.src = get_src( argv[ 1 ], &Compilier );

	Compilier.mode = first;
	compile( &Compilier );
	Compilier.mode = second;
	compile( &Compilier );

	makecode( &Compilier );

	return 0;
}

int Compilier_constr( struct Compilier_structure *Compilier )
{
	Compilier -> src_sz = 0;
	Compilier -> src = NULL;
	Compilier -> exe_sz = 0;
	Compilier -> labels =
		( struct labels_t ** )calloc( label_amount, sizeof( struct labels_t * ) );
	Compilier -> lbl_count = 0;
	Compilier -> exe = NULL;
	Compilier -> exe_sz = 0;
}

char *get_src( char *src_file_name, struct Compilier_structure *Compilier )
{
	FILE *src_file = fopen( src_file_name, "r" );
	Compilier -> src_sz = srcSize( src_file );
	char *src = ( char * )calloc( sizeof( char ), Compilier -> src_sz );
	fread( src, sizeof( char ), Compilier -> src_sz, src_file );
	fclose( src_file );
	Compilier -> exe = calloc( Compilier -> src_sz, sizeof( int/*double*/ ) );
	return src;
}

size_t srcSize( FILE *src )
{
	fseek( src, 0, SEEK_END );
	size_t src_sz = ftell( src );
	rewind( src );
	return src_sz;
}

int compile( struct Compilier_structure *Compilier )
{
	#define DEF_CMD( NAME,  name, num, Cmplr_code2, Cmplr_code1, elf, CPU_code )	\
		const int CMD_##NAME = num;

	#include <commands.h>

	#undef DEF_CMD

	void *exe_cur = Compilier -> exe;
	int src_cur = 0;
	int src_cur_delta = 0;

	if( Compilier -> mode == first )
	{
        int elf_cur = 0;
		while( src_cur < Compilier -> src_sz )
		{
			char *str = ( char * )calloc( 10, sizeof( char ) );
			from_src( %s, str );
			#define DEF_CMD( NAME, name, num, Cmplr_code2, Cmplr_code1, elf, CPU_code )		\
			do										\
			{										\
				if( strcmp( str, #name ) == 0 )						\
				{									\
					exe_cur += Cmplr_code1;						\
                    elf_cur += elf;                 \
				}									\
			}while( 0 )

			#include <commands.h>

			#undef DEF_CMD

			free( str );
		}
		return 0;
	}
	
	if( Compilier -> mode == second )
	{
		while( src_cur < Compilier -> src_sz )							
		{
			char *str = ( char * )calloc( 10, sizeof( char ) );
			from_src( %s, str );
			#define DEF_CMD( NAME, name, num, Cmplr_code2, Cmplr_code1, elf,  CPU_code )	\
			do										\
			{										\
				if( strcmp( str, #name ) == 0 )						\
				{									\
					Cmplr_code2;							\
				}									\
			}while( 0 )

			#include <commands.h>

			#undef DEF_CMD

			free( str );
		}
		Compilier -> exe_sz = ( size_t ) exe_cur - ( size_t )( Compilier -> exe );

		return 0;
	}
}

int makecode( struct Compilier_structure *Compilier )
{
	FILE *exe_file = fopen( "exe", "w" );
	fwrite( Compilier -> exe, Compilier -> exe_sz, sizeof( char ), exe_file );
	fclose( exe_file );
}
