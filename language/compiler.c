#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./../log/log.h"
#include "./../tree_t/tree.h"
#include "lex_analyser.h"
#include "LNG_recurs_desc_parser.h"
#include "back_end.h"
#include "compiler.h"

int main( int argc, char *argv[] )
{
	log_constr();

	struct analyser_t analyser;

	analyser_constr( &analyser );

	get_src( &analyser, argv[ 1 ] );

	analyser_make_lexems( &analyser );

	analyser_make_tree( &analyser );

	dumper( analyser.tree );

	//analyser_make_asm( &analyser );

	log_destr();

	return 0;
}

size_t get_src_sz( FILE *src_file );

int get_src( struct analyser_t *analyser, char *src )
{
	check_pointer( analyser, 1 );
	check_pointer( src, 1 );

	FILE *src_file = fopen( src, "r" );
	check_pointer( src_file, 1 );

	analyser -> src_sz = get_src_sz( src_file );

	analyser -> src = ( char * )calloc( analyser -> src_sz, sizeof( char ) );

	fread( analyser -> src, sizeof( char ), analyser -> src_sz, src_file );

	fclose( src_file );
}

size_t get_src_sz( FILE *src_file )
{
	check_pointer( src_file, 0 );

	fseek( src_file, 0, SEEK_END );

	size_t sz = ftell( src_file );

	rewind( src_file );

	return sz;
}
