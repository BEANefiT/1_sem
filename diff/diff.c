#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../log/log.h"
#include "./../tree_t/tree.h"
#include "diff.h"

const int MAX_PHRASE_LENGTH = 37;

#define OPER_DEF( cmd, operatr, code ) cmd,

enum operator_t
{
	#include "operators.h"
	end
};

#undef OPER_DEF


struct element
{
	enum operator_t oper;
	enum type_t 	type;
	double 		value;
	char*		var;
};

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

	Do( start( &akinator ) );

	Do( dumper( akinator.newtree ) );
	log_destr();
	return 0;
}

int aki_constr( struct aki_structure *akinator )
{
	check_pointer( akinator, 1 );

	akinator -> src_sz = 0;
	akinator -> src = NULL;
	akinator -> tree = NULL;
	akinator -> newtree = NULL;
	akinator -> src_cur = 3;
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

int type_recognize( struct element *node, char *str )
{
	node -> type = variable;
	node -> var = ( char * )calloc( strlen( str ), sizeof( char ) );
	strcpy( node -> var, str );
	
	#define OPER_DEF( cmd, operatr, code )		\
	do						\
	{						\
		if( !strcmp( str, #operatr ) )		\
		{					\
			node -> type = operat;		\
			node -> oper = cmd;		\
			return 0;			\
		}					\
	}while( 0 );

	#include "operators.h"
	#undef OPER_DEF
}


int read_base_elem( struct aki_structure *akinator, struct tree_node_t *node, enum side_t side )
{
	double koeff = 0;
	char *str = ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );
	size_t src_cur_delta = 0;
	sscanf( akinator -> src + akinator -> src_cur, "%[^\'\"]%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;
	sscanf( akinator -> src + akinator -> src_cur, "%*1[\'\"]%lg%*1[\'\"]%n", &koeff, &src_cur_delta );
	struct element *newnode = NULL;
	if( src_cur_delta != 0 )
	{
		newnode = ( struct element * )calloc( 1, sizeof( struct element ) );
		newnode -> value = koeff;
		newnode -> type = koefficient;
	}
	if( src_cur_delta == 0 )
	{
		sscanf( akinator -> src + akinator -> src_cur, "%*1[\'\"]%[^\'\"]%*1[\'\"]%n", str, &src_cur_delta );
		newnode = ( struct element * )calloc( 1, sizeof( struct element ) );
		type_recognize( newnode, str );
		
	}
	akinator -> src_cur += src_cur_delta;

	Do( make_node( akinator, tree_add( akinator -> tree, node, side, newnode ) ) );
}

int make_tree( struct aki_structure *akinator )
{
	check_pointer( akinator, 1 );
	
	double koeff = 0;
	char *str = ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );
	size_t src_cur_delta = 0;

	sscanf( akinator -> src + akinator -> src_cur, "%[^\'\"]%n", str, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;
	sscanf( akinator -> src + akinator -> src_cur, "%*1[\'\"]%lg%*1[\'\"]%n", &koeff, &src_cur_delta );
	struct element *root = NULL;
	if( src_cur_delta != 0 )
		{
			root = ( struct element * )calloc( 1, sizeof( struct element ) );
			root -> value = koeff;
			root -> type = koefficient;
		}
	if( src_cur_delta == 0 )
		{
			sscanf( akinator -> src + akinator -> src_cur, "%*1[\'\"]%[^\'\"]%*1[\'\"]%n", str, &src_cur_delta );
			root = ( struct element * )calloc( 1, sizeof( struct element ) );
			type_recognize( root, str );
		}
	akinator -> src_cur += src_cur_delta;
	tree_create( struct element, tree, struct_element, root );
	akinator -> tree = tree;
	Do( make_node( akinator, tree_get_root( akinator -> tree ) ) );
}

struct tree_node_t *num( struct tree_t *tree, double val );

struct tree_node_t *diff( struct tree_t *tree, struct tree_node_t *node )
{
	check_pointer( tree, NULL );
	check_pointer( node, NULL );

	if( isNUM ) return num( tree, 0 );

	if( isVAR ) return num( tree, 1 );

	switch( OPERAT )
	{
		#define OPER_DEF( cmd, operatr, code )	\
		do					\
		{					\
			case cmd:			\
			{				\
				return code;		\
			}				\
		}while( 0 );

		#include "operators.h"
		#undef OPER_DEF
	}

}

struct tree_node_t *num( struct tree_t *tree, double val )
{
	struct element *elem = ( struct element * )calloc( 1, sizeof( struct element ) );
	elem -> type = koefficient;
	elem -> value = val;

	return tree_node_construct( tree, NULL, elem );
}

int print_struct_element( FILE *out, void *elem )
{
	switch( ( ( struct element *)elem ) -> type )
	{
		case koefficient:
		{
			fprintf( out, "%lg", (( struct element * )elem ) -> value );
			break;
		}
		case variable:
		{
			fprintf( out, "%s", (( struct element * )elem ) -> var );
			break;
		}
		case operat:
		{
			switch( ( ( struct element * )elem ) -> oper )
			{
				#define OPER_DEF( cmd, operatr, code )	\
				case cmd:				\
				{					\
					fprintf( out, #operatr );	\
					break;				\
				}					\
	
				#include "operators.h"
				#undef OPER_DEF
			}
	
			break;
		}
	}
}

#define OPERAT_CODE( NAME, symbol )								\
												\
struct tree_node_t *NAME( struct tree_t *tree, struct tree_node_t *a, struct tree_node_t *b )	\
{												\
	struct element *elem = ( struct element * )calloc( 1, sizeof( struct element ) );	\
	elem -> type = operat;									\
	elem -> oper = symbol;									\
	struct tree_node_t *parent = tree_node_construct( tree, NULL, elem );			\
	tree_set( parent, left, a );								\
	tree_set( parent, right, b );								\
	return parent;										\
}												\

OPERAT_CODE( ADD, add_cmd )
OPERAT_CODE( SUB, sub_cmd )
OPERAT_CODE( DIV, div_cmd )
OPERAT_CODE( MUL, mul_cmd )

#undef OPERAT_CODE

int start( struct aki_structure *akinator )
{
	check_pointer( akinator, 1 );
	
	Do( tree_create( struct element, newtree, struct_element, diff( akinator -> tree, tree_get_root( akinator -> tree ) ) ) );
	
}


/*int base_write_node( FILE* base, struct tree_node_t *node, int tab_counter )
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
}*/
