#include <stdio.h>
#include <stdlib.h>
#include "./../tree_t/tree.h"
#include "akinator.h"

int main( int argc, char *argv[] )
{
	struct aki_structure akinator;
	aki_constr( &akinator );
	akinator.src = getsrc( argv[ 1 ], &akinator );
	make_tree( &akinator );
	//dumper( akinator.tree );
	return 0;
}

int aki_constr( struct aki_structure *akinator )
{
	akinator -> src_sz = 0;
	akinator -> src = NULL;
	akinator -> tree = NULL;
	akinator -> src_cur = 1;
}

char *getsrc( char *src_name, struct aki_structure *akinator )
{
	FILE *src_file = fopen( src_name, "r" );
	akinator -> src_sz = src_sz( src_file );
	char *src = ( char * )calloc( sizeof( char ), akinator -> src_sz );
	fread( src, sizeof( char ), akinator -> src_sz, src_file );
	fclose( src_file );
	return src;
}

size_t src_sz( FILE *src )
{
	fseek( src, 0, SEEK_END );
	size_t sz = ftell( src );
	rewind( src );
	return sz;
}

int make_node( struct aki_structure *akinator, struct tree_node_t *node )
{
	char *str = ( char * )calloc( 99, sizeof( char ) );
	size_t src_cur_delta = 0;
	sscanf( akinator -> src + akinator -> src_cur, "%[^()]%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;
	char tmp = ' ';
	sscanf( akinator -> src + akinator -> src_cur, "%c", &tmp );
	akinator -> src_cur ++;
	if( tmp == '(' )
	{ 
		sscanf( akinator -> src + akinator -> src_cur, "\'%[^\']\'%n", str, &src_cur_delta );
		akinator -> src_cur += src_cur_delta;
		printf( "'%s'\n", str );
		make_node( akinator, tree_add( akinator -> tree, node, left, str ) );
		free( str );
	}
	if( tmp == ')' )
	{
		return 0;
	}
	str = ( char * )calloc( 99, sizeof( char ) );
	sscanf( akinator -> src + akinator -> src_cur, "%[^()]%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;
	sscanf( akinator -> src + akinator -> src_cur, "%c", &tmp );
	akinator -> src_cur ++;
	if( tmp == '(' )
	{
		sscanf( akinator -> src + akinator -> src_cur, "\'%[^\']\'%n", str, &src_cur_delta );
		akinator -> src_cur += src_cur_delta;
		printf( "'%s'\n", str );
		make_node( akinator, tree_add( akinator -> tree, node, right, str ) );
		free( str );
	}
	str = ( char * )calloc( 99, sizeof( char ) );
	sscanf( akinator -> src + akinator -> src_cur, "%[^()]%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;
	sscanf( akinator -> src + akinator -> src_cur, "%c", &tmp );
	akinator -> src_cur ++;
	free( str );
	if( tmp == ')' )
	{
		return 0;
	}
}

int make_tree( struct aki_structure *akinator )
{
	char str[ 99 ] = {};
	size_t src_cur_delta = 0;
	sscanf( akinator -> src + akinator -> src_cur, "\'%[^\']\'%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;
	struct tree_node_t *root = tree_construct( &( akinator -> tree ), "POISON", str );
	make_node( akinator, root );
	dumper( akinator -> tree );
}
