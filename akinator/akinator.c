#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __TREE_CHAR__
#include <log.h>
#include <tree.h>
#include "akinator.h"
#include <stack.h>

int main( int argc, char *argv[] )
{
	log_constr();
	struct aki_structure akinator;
	aki_constr( &akinator );
	akinator.src = getsrc( argv[ 1 ], &akinator );
	make_tree( &akinator );
	get_command( &akinator, argv[ 1 ] );
	Do( dumper( akinator.tree ) );
	log_destr();
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

int get_command( struct aki_structure *akinator, char *base_name )
{
	printf( "Enter cmd( (s)tart, (d)etermine, (b)ase_edit, (e)scape )\n" );
	char cmd;
	scanf( "%c", &cmd );
	switch( cmd )
	{
		case 's':
		{
			start( akinator );
			break;
		}
		case 'd':
		{
			determine( akinator );
			break;
		}
		case 'b':
		{
			base_edit( akinator, base_name );
			break;
		}
		case 'e':
		{
			return 1;
		}
	}
	printf( "....\n" );
	
	if( get_command( akinator, base_name ) == 1 )
		return 1;
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
		tree_node_t *newnode = NULL;
		Do( newnode = tree_add( akinator -> tree, node, left, str ) );
		make_node( akinator, newnode );
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
		tree_node_t *newnode = NULL;
		Do( newnode = tree_add( akinator -> tree, node, right, str ) );
		make_node( akinator, newnode );
	}
	str = ( char * )calloc( 99, sizeof( char ) );
	sscanf( akinator -> src + akinator -> src_cur, "%[^()]%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;
	sscanf( akinator -> src + akinator -> src_cur, "%c", &tmp );
	akinator -> src_cur ++;
	if( tmp == ')' )
	{
		return 0;
	}
}

int make_tree( struct aki_structure *akinator )
{
	char *str = ( char * )calloc( 99, sizeof( char ) );
	size_t src_cur_delta = 0;
	sscanf( akinator -> src + akinator -> src_cur, "\'%[^\']\'%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;
	str = ( char * )realloc( str, src_cur_delta );
	tree_node_t *root = NULL;
	Do( root = tree_construct( &( akinator -> tree ), str ) );
	make_node( akinator, root );
}

char get_answer()
{
	char answer = ' ';
	scanf( "%c", &answer );
	if( answer == 'y' || answer == 'Y' || answer == '+' )
		return 'y';
	if( answer == 'n' || answer == 'N' || answer == '-' )
		return 'n';
	char tmp = get_answer();
	return tmp;
}

int ask( tree_node_t *current, tree_node_t **end )
{
	if( tree_get_next( current, left ) != NULL && tree_get_next( current, right ) != NULL )
	{
		printf( "%s?( (y)es, (n)o )\n", tree_get_elem( current ) );
		char answer = get_answer();
		if( answer == 'n' )
			ask( tree_get_next( current, left ), end );
		if( answer == 'y' )
			ask( tree_get_next( current, right ), end );
	}
	else
	{
		printf( "This is %s?( (y)es, (n)o )\n", tree_get_elem( current ) );
		*end = current;
	}
}

int start( struct aki_structure *akinator )
{
	struct tree_node_t *end = NULL;
	ask( tree_get_root( akinator -> tree ), &end );
	char answer = get_answer();
	if( answer == 'y' )
	{
		printf( "ok\n" );
		return 0;
	}
	if( answer == 'n' )
	{
		printf( "What is it?\n" );
		char *str = ( char *)calloc( 99, sizeof( char ) );
		scanf( "%s", str );
		tree_add( akinator -> tree, end, left, tree_get_elem( end ) );
		tree_add( akinator -> tree, end, right, str );
		printf( "What differs %s from %s\n", str, tree_get_elem( end ) );
		char *differ = ( char * )calloc( 99, sizeof( char ) );
		scanf( "%s", differ );
		change_elem( end, differ );
	}
}

int base_write_node( FILE* base, struct tree_node_t *node, int tab_counter )
{
	for( int i = 0; i < tab_counter; i++ )
		fprintf( base, "\t" );
	fprintf( base, "('%s'\n", tree_get_elem( node ) );
	if( tree_get_next( node, left ) == NULL || tree_get_next( node, right ) == NULL )
	{
		for( int i = 0; i < tab_counter; i++ )
			fprintf( base, "\t" );
		fprintf( base, ")\n" );
		return 0;
	}
	base_write_node( base, tree_get_next( node, left ), tab_counter + 1 );
	base_write_node( base, tree_get_next( node, right ), tab_counter + 1 );
	for( int i = 0; i < tab_counter; i++ )
		fprintf( base, "\t" );
	fprintf( base, ")\n" );
}

int base_edit( struct aki_structure *akinator, char *base_name )
{
	FILE *base = fopen( base_name, "w" );
	base_write_node( base, tree_get_root( akinator -> tree ), 0 );
	fclose( base );
}

int determine( struct aki_structure *akinator )
{
	printf( "Type character\n" );
	char *name = ( char * )calloc( 50, sizeof( char ) );
	scanf( "%s", name );
	struct tree_node_t *object = tree_find( tree_get_root( akinator -> tree ), name );
	stack( struct tree_node_t *, node_ptrs );
	int kostyl = 0;
	while( !kostyl )
	{
		if( object == tree_get_root( akinator -> tree ) )
			kostyl = 1;
		Do( push( node_ptrs, &object ) );
		object = tree_get_parent( object );
	}
	size_t sz = size( node_ptrs );
	for( size_t i = 1; i < sz; i++ )
	{
		struct tree_node_t *tmp = NULL;
		struct tree_node_t *next = NULL;
		Do( pop( node_ptrs, &tmp ) );
		Do( top( node_ptrs, &next ) );
		if( tree_get_next( tmp, left ) == next )
		{
			printf( "ne " );
		}
		printf( "%s", tree_get_elem( tmp ) );
		if( strcmp( tree_get_elem( next ), name) )
			printf( ", " );
	}
	printf( "\n" );
}
