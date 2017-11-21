#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <log.h>
#include <tree.h>
#include "akinator.h"
#include <stack.h>

const int MAX_PHRASE_LENGTH = 256;

int main( int argc, char *argv[] )
{
	log_constr();

	struct aki_structure akinator;
	Do( aki_constr( &akinator ) );

	if( argc == 1 )
	{
		printf( "ERROR: Enter base name\n" );
		return 0;
	}
	Do( akinator.src = getsrc( argv[ 1 ], &akinator ) );
	
	if( akinator.src == NULL )
	{
		printf( "ERROR: '%s' no such file or directory\n", argv[ 1 ] );
		return 0;
	}

	Do( make_tree( &akinator ) );

	printf( "Hello! Tap Enter to get started :)\n" );
	Do( get_command( &akinator, argv[ 1 ] ) );

	Do( dumper( akinator.tree ) );
	log_destr();
	return 0;
}

int aki_constr( struct aki_structure *akinator )
{
	check_pointer( akinator, 1 );

	akinator -> src_sz = 0;
	akinator -> src = NULL;
	akinator -> tree = NULL;
	akinator -> src_cur = 0;
}

char *getsrc( char *src_name, struct aki_structure *akinator )
{
	check_pointer( src_name, NULL );
	check_pointer( akinator, NULL );

	FILE *src_file = fopen( src_name, "r" );
	if( src_file == NULL )
	{
		print_log( "ERROR: BASE NOT FOUND\n" );
		return NULL;
	}

	char separator = fgetc( src_file );
	if( separator != '\'' && separator != '\"' )
	{
		printf( "invalid separator\n" );
		return NULL;
	}
	akinator -> src_cur = 2;
	Do( akinator -> src_sz = src_sz( src_file ) );
	char *src = ( char * )calloc( sizeof( char ), akinator -> src_sz );
	fread( src, sizeof( char ), akinator -> src_sz, src_file );
	fclose( src_file );
	return src;
}

size_t src_sz( FILE *src )
{
	check_pointer( src, 0 );

	fseek( src, 0, SEEK_END );
	size_t sz = ftell( src );
	rewind( src );
	return sz;
}

int get_command( struct aki_structure *akinator, char *base_name )
{
	check_pointer( akinator, 1 );
	check_pointer( base_name, 1 );

	while( getchar() != '\n' )
		;
	printf( "Enter cmd( (s)tart, (d)etermine, (c)ompare, (b)ase_edit, (p)rint_base, (q)uit )\n" );
	char cmd = getchar();
	switch( cmd )
	{
		case 's':
		{
			Do( start( akinator ) );
			break;
		}
		case 'd':
		{
			Do( determine( akinator ) );
			break;
		}
		case 'b':
		{
			Do( base_edit( akinator, base_name ) );
			break;
		}
		case 'p':
		{
			Do( print_base( akinator ) );
			break;
		}
		case 'q':
		{
			return 1;
		}
		case 'c':
		{
			Do( compare( akinator ) );
			break;
		}
	}
	printf( "\n\n*******************************************************************************\n" );
	
	if( get_command( akinator, base_name ) == 1 )
		return 1;
}

int read_base( struct aki_structure *akinator, char *tmp )
{
	char *str = ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );
	size_t src_cur_delta = 0;

	sscanf( akinator -> src + akinator -> src_cur, "%[^()]%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;
	sscanf( akinator -> src + akinator -> src_cur, "%c", tmp );
	akinator -> src_cur ++;

	free( str );
}

int read_base_elem( struct aki_structure *akinator, struct tree_node_t *node, enum side_t side );

int make_node( struct aki_structure *akinator, struct tree_node_t *node )
{
	check_pointer( akinator, 1 );
	check_pointer( node, 1 );


	char tmp = ' ';

	read_base( akinator, &tmp );
	if( tmp == '(' )
	{ 
		read_base_elem( akinator, node, left );
	}
	if( tmp == ')' )
	{
		return 0;
	}

	read_base( akinator, &tmp );
	if( tmp == '(' )
	{
		read_base_elem( akinator, node, right );
	}

	read_base( akinator, &tmp );
	if( tmp == ')' )
	{
		return 0;
	}
}

int read_base_elem( struct aki_structure *akinator, struct tree_node_t *node, enum side_t side )
{
	char *str = ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );
	size_t src_cur_delta = 0;
	sscanf( akinator -> src + akinator -> src_cur, "%*1[\'\"]%[^\'\"]%*1[\'\"]%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;
	str = ( char * )realloc( str, src_cur_delta );

	Do( make_node( akinator, tree_add( akinator -> tree, node, side, &str ) ) );
}

int make_tree( struct aki_structure *akinator )
{
	check_pointer( akinator, 1 );

	char *str = ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );
	size_t src_cur_delta = 0;

	sscanf( akinator -> src + akinator -> src_cur, "%[^\'\"]%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;
	sscanf( akinator -> src + akinator -> src_cur, "%*1[\'\"]%[^\'\"]%*1[\'\"]%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;

	str = ( char * )realloc( str, src_cur_delta );
	tree_create( char *, tree, string, &"POISON" );

	akinator -> tree = tree;
	change_elem( akinator -> tree, tree_get_root( akinator -> tree ), &str );

	Do( make_node( akinator, tree_get_root( akinator -> tree ) ) );
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

int ask( struct tree_t *tree, struct tree_node_t *current, struct tree_node_t **end )
{
	check_pointer( current, 1 );
	check_pointer( end, 1 );

	if( tree_get_next( current, left ) != NULL && tree_get_next( current, right ) != NULL )
	{
		tree_print( tree, stdout, tree_get_elem( current ) );
		printf( "?( (y)es, (n)o )\n" );
		char answer = get_answer();
		if( answer == 'n' )
			ask( tree, tree_get_next( current, left ), end );
		if( answer == 'y' )
			ask( tree, tree_get_next( current, right ), end );
	}
	else
	{
		printf( "This is " );
		tree_print( tree, stdout, tree_get_elem( current ) );
		printf( "?( (y)es, (n)o )\n" );
		*end = current;
	}
}

int start( struct aki_structure *akinator )
{
	check_pointer( akinator, 1 );

	struct tree_node_t *end = NULL;
	Do( ask( akinator -> tree, tree_get_root( akinator -> tree ), &end ) );
	char answer = get_answer();
	if( answer == 'y' )
	{
		printf( "ok\n" );
		return 0;
	}
	if( answer == 'n' )
	{
		printf( "What is it?\n" );

		while( getchar() != '\n' )
			;
		
		char *str = ( char *)calloc( MAX_PHRASE_LENGTH, sizeof( char ) );
		str = ( char * )calloc( strlen( str ), sizeof( char ) );
		fgets( str, MAX_PHRASE_LENGTH, stdin );
		str[ strlen( str ) - 1 ] = '\0';

		Do( tree_add( akinator -> tree, end, left, tree_get_elem( end ) ) );
		Do( tree_add( akinator -> tree, end, right, &str ) );
		printf( "What differs %s from ", str );
		Do( tree_print( akinator -> tree, stdout, tree_get_elem( end ) ) );
		printf( "\n" );
		
		char *differ = ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );
		scanf( "%s", differ );
		Do( change_elem( akinator -> tree, end, &differ ) );
	}
}

int base_write_node( FILE* base, struct tree_node_t *node, int tab_counter )
{
	check_pointer( base, 1 );
	check_pointer( node, 1 );

	for( int i = 0; i < tab_counter; i++ )
		fprintf( base, "\t" );
	fprintf( base, "('%s'\n", *( char ** )( tree_get_elem( node ) ) );
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
	check_pointer( akinator, 1 );
	check_pointer( base_name, 1 );

	FILE *base = fopen( base_name, "w" );
	fprintf( base, "'\n" );
	Do( base_write_node( base, tree_get_root( akinator -> tree ), 0 ) );
	fclose( base );
}

struct tree_node_t *make_path( struct aki_structure *akinator, struct stack_t *stack_name, char **name, char *text );

int determine( struct aki_structure *akinator )
{
	check_pointer( akinator, 1 );

	char *name = NULL;
	stack( struct tree_node_t *, node_ptrs );
	struct tree_node_t *object = make_path( akinator, node_ptrs, &name, "Type character" );
	check_pointer( object, 1 );

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
		tree_print( akinator -> tree, stdout, tree_get_elem( tmp ) );
		if( strcmp( *( char ** )tree_get_elem( next ), name) )
			printf( ", " );
	}
	printf( "\n" );
}

int print_base( struct aki_structure *akinator )
{
	Do( dumper( akinator -> tree ) );
	check_pointer( akinator, 1 );
	system( "dot -q -Tpdf dump -o dmp\n" );
	system( "alias evince='evince 2>/dev/null'\n" );
	system( "evince dmp" );
	return 0;
}

int compare( struct aki_structure *akinator )
{
	check_pointer( akinator, 1 );

	char *name1 = NULL;
	stack( struct tree_node_t *, node_ptrs1 );
	struct tree_node_t *object1 = make_path( akinator, node_ptrs1, &name1, "Type 1st character" );
	check_pointer( object1, 1 );

	char *name2 = NULL;
	stack( struct tree_node_t *, node_ptrs2 );
	struct tree_node_t *object2 = make_path( akinator, node_ptrs2, &name2, "Type 2nd character" );
	check_pointer( object2, 1 );

	struct tree_node_t *tmp1 = NULL;
	struct tree_node_t *tmp2 = NULL;
	struct tree_node_t *tmp3 = NULL;
	struct tree_node_t *tmp4 = NULL;

	Do( pop( node_ptrs1, &tmp1 ) );
	Do( pop( node_ptrs2, &tmp2 ) );
	Do( top( node_ptrs1, &tmp3 ) );
	Do( top( node_ptrs2, &tmp4 ) );
	Do( push( node_ptrs1, &tmp1 ) );
	Do( push( node_ptrs2, &tmp2 ) );

	if( tmp3 == tmp4 )
	{
		printf( "%s and %s are both ", name1, name2 );
		char *spk = ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );
		sprintf( spk, "espeak \' %s and %s are both \' -s 135", name1, name2 );
		system( spk );
		free( spk );
	}
	else
	{
		printf( "They are different at all. \n" );
		system( "espeak \"They are different at all\" -s 135" );
	}

	for( int i = 0; i < ( size( node_ptrs1 ) > size( node_ptrs2 ) ? size( node_ptrs1 ) : size( node_ptrs2 ) ); i++ )
	{
		struct tree_node_t *current1 = NULL;
		struct tree_node_t *current2 = NULL;
		struct tree_node_t *next1 = NULL;
		struct tree_node_t *next2 = NULL;

		Do( pop( node_ptrs1, &current1 ) );
		Do( pop( node_ptrs2, &current2 ) );
		Do( top( node_ptrs1, &next1 ) );
		Do( top( node_ptrs2, &next2 ) );

		if( next1 == next2 && next1 != NULL )
		{
			printf( "%s, ", *( char ** )tree_get_elem( current1 ) );

			char *spk = ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );
			sprintf( spk, "espeak \' %s, \' -s 135", *( char ** )tree_get_elem( current1 ) );
			system( spk );
			free( spk );

			continue;
		}
		if( current1 == current2 )
		{
			tree_get_next( current1, right ) == next1
			?
				printf( "%s is %s, but %s is not.", name1, *( char ** )tree_get_elem( current1 ), name2 )
			:
				printf( "%s is %s, but %s is not.", name2, *( char ** )tree_get_elem( current1 ), name1 );
			
			char *spk = ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );
			
			tree_get_next( current1, right ) == next1
			?
				sprintf( spk, "espeak \'%s is %s, but %s is not.\' -s 135", name1, *( char ** )tree_get_elem( current1 ), name2 )
			:
				sprintf( spk, "espeak \'%s is %s, but %s is not.\' -s 135", name2, *( char ** )tree_get_elem( current1 ), name1 );

			system( spk );
			free( spk );

			break;
		}
	}
}

struct tree_node_t *make_path( struct aki_structure *akinator, struct stack_t *stack_name, char **name, char *text )
{
	check_pointer( akinator, NULL );
	check_pointer( stack_name, NULL );
	check_pointer( name, NULL );
	check_pointer( text, NULL );

	printf( "%s\n", text );
	*name =  ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );
	scanf( "%s", *name );
	*name = ( char * )realloc( *name, strlen( *name ) );

	struct tree_node_t *object = NULL;
	Do( object = tree_find( akinator -> tree, tree_get_root( akinator -> tree ), name ) );

	if( object == NULL )
	{
		printf( "%s not found :(\n", *name );
		return NULL;
	}

	int kostyl = 0;
	while( !kostyl )
	{
		Do( push( stack_name, &object ) );

		if( object == tree_get_root( akinator -> tree ) )
			kostyl = 1;

		else
			Do( object = tree_get_parent( object ) );
	}

	return object;
}
