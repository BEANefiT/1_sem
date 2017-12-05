#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "./../log/log.h"
#include "./../tree_t/tree.h"
#include "diff.h"

const int MAX_PHRASE_LENGTH = 37;

#define OPER_DEF( cmd, type, FUNC, operatr, code ) cmd,

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
	char *		var;
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
	Do( getsrc( argv[ 1 ], &akinator ) );
	
	if( akinator.buffer == NULL )
	{
		printf( "ERROR: '%s' no such file or directory\n", argv[ 1 ] );
		return 0;
	}

	Do( make_tree( &akinator ) );

	Do( akinator.newtree = start( &akinator ) );

	Do( dumper( akinator.newtree ) );

	tex_it( &akinator );
	log_destr();
	return 0;
}

int aki_constr( struct aki_structure *akinator )
{
	check_pointer( akinator, 1 );

	akinator -> buf_sz = 0;
	akinator -> buffer = NULL;
	akinator -> tree = NULL;
	akinator -> newtree = NULL;
	akinator -> src_cur = 0;
	akinator -> dvar = NULL;
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

	size_t src_sz = get_file_sz( src_file );

	char *src = ( char * )calloc( src_sz, sizeof( char ) );
	fread( src, sizeof( char ), src_sz, src_file );
	fclose( src_file );

	akinator -> buffer = ( char * )calloc( src_sz, sizeof( char ) );

	for( size_t i = 0; i < src_sz; i++ )
		if( src[ i ] != ' ' && src[ i ] != '\n' && src[ i ] != '\t' )
			akinator -> buffer[ akinator -> buf_sz ++ ] = src[ i ];

	akinator -> buffer = ( char * )realloc( akinator -> buffer, akinator -> buf_sz * sizeof( char ) );
}

size_t get_file_sz( FILE *src )
{
	check_pointer( src, 0 );

	fseek( src, 0, SEEK_END );
	size_t sz = ftell( src );
	rewind( src );
	return sz;
}

int make_tree( struct aki_structure *akinator )
{
	check_pointer( akinator, 1 );

	struct element *root_tmp = ( struct element * )calloc( 1, sizeof( struct element ) );
	tree_create( struct element, tree, struct_element, root_tmp );
	akinator -> tree = tree;

	struct tree_node_t *root = getG( akinator );
	
	tree_node_change( tree_get_root( akinator -> tree ), root );

	akinator -> dvar = ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );

	sscanf( akinator -> buffer + akinator -> src_cur + 1, "%[a-zA-Z]", akinator -> dvar );

	akinator -> dvar = ( char * )realloc( akinator -> dvar, ( strlen( akinator -> dvar ) + 1 ) * sizeof( char ) );
}

struct tree_node_t *getG( struct aki_structure *akinator )
{
	return getE( akinator );
}

struct tree_node_t *getE( struct aki_structure *akinator )
{
	struct tree_node_t *node = getT( akinator );

	while( ( akinator -> buffer[ akinator -> src_cur ] == '+' ) || ( akinator -> buffer[ akinator -> src_cur ] == '-' ) )
	{
		char oper = akinator -> buffer[ akinator -> src_cur ++ ];

		struct tree_node_t *node2 = getT( akinator );

		if( oper == '+' )
		{
			struct tree_node_t *node_cp = tree_copy( akinator -> tree, node );

			node = tree_node_change( node, ADD( akinator -> tree, node_cp, node2 ) );
		}

		else
		{
			struct tree_node_t *node_cp = tree_copy( akinator -> tree, node );

			node = tree_node_change( node, SUB( akinator -> tree, node_cp, node2 ) );
		}
	}

	return node;
}

struct tree_node_t *getT( struct aki_structure *akinator )
{
	struct tree_node_t *node = getB( akinator );

	while( ( akinator -> buffer[ akinator -> src_cur ] == '*' ) || ( akinator -> buffer[ akinator -> src_cur ] == '/' ) )
	{
		char oper = akinator -> buffer[ akinator -> src_cur ++ ];

		struct tree_node_t *node2 = getB( akinator );

		if( oper == '*' )
		{
			struct tree_node_t *node_cp = tree_copy( akinator -> tree, node );

			node = tree_node_change( node, MUL( akinator -> tree, node_cp, node2 ) );
		}

		else
		{
			struct tree_node_t *node_cp = tree_copy( akinator -> tree, node );

			node = tree_node_change( node, DIV( akinator -> tree, node_cp, node2 ) );
		}
	}

	return node;
}

struct tree_node_t *getB( struct aki_structure *akinator )
{
	struct tree_node_t *node = getP( akinator );

	while( akinator -> buffer[ akinator -> src_cur ] == '^' )
	{
		akinator -> src_cur ++;

		struct tree_node_t *node_cp = tree_copy( akinator -> tree, node );

		struct tree_node_t *node2 = getP( akinator );

		node = tree_node_change( node, POW( akinator -> tree, node_cp, node2 ) );
	}

	return node;
}

struct tree_node_t *getP( struct aki_structure *akinator )
{
	if( akinator -> buffer[ akinator -> src_cur ] == '(' )
	{
		akinator -> src_cur ++;

		struct tree_node_t *node = getE( akinator );

		assert( akinator -> buffer[ akinator -> src_cur++ ] == ')' );

		return node;
	}

	struct tree_node_t *node = getN( akinator );

	if( node == NULL )
		node = getF( akinator );

	return node;
}

struct tree_node_t *getF( struct aki_structure *akinator )
{
	size_t src_cur_delta = 0;
	char *cmd_name = ( char * )calloc( MAX_PHRASE_LENGTH, sizeof( char ) );

	sscanf( akinator -> buffer + akinator -> src_cur, "%[a-zA-Z]%n", cmd_name, &src_cur_delta );
	akinator -> src_cur += src_cur_delta;

	struct element *func = ( struct element * )calloc( 1, sizeof( struct element ) );
	func -> type = variable;

	#define OPER_DEF( cmd, cmd_type, FUNC, operatr, code )			\
	if( !strcmp( #cmd_type, "uno" ) && !strcmp( #operatr, cmd_name ) )	\
	{									\
		func -> type = operat;						\
		func -> oper = cmd;						\
	}

	#include "operators.h"
	#undef OPER_DEF

	if( func -> type == variable )
	{
		func -> var = ( char * )calloc( strlen( cmd_name ) + 1, sizeof( char ) );
		
		strcpy( func -> var, cmd_name );
	}

	struct tree_node_t *node = tree_node_construct( akinator -> tree, NULL, func );
	
	if( func -> type == operat )
	{
		if( akinator -> buffer[ akinator -> src_cur ] == '(' )
		{
			akinator -> src_cur ++;
			tree_set( node, left, getE( akinator ) );
	
			assert( akinator -> buffer[ akinator -> src_cur ++ ] == ')' );
		}
		
		else
			tree_set( node, left, getN( akinator ) );
	}

	return node;
}

struct tree_node_t *getN( struct aki_structure *akinator )
{
	struct element *elem = ( struct element * )calloc( 1, sizeof( struct element ) );
	elem -> type = koefficient;

	size_t src_cur_delta = 0;

	sscanf( akinator -> buffer + akinator -> src_cur, "%lg%n", &( elem -> value ), &src_cur_delta );

	if( src_cur_delta == 0 )
		return NULL;

	akinator -> src_cur += src_cur_delta;

	return tree_node_construct( akinator -> tree, NULL, elem );
}

int type_recognize( struct element *node, char *str )
{
	node -> type = variable;
	node -> var = ( char * )calloc( strlen( str ) + 1, sizeof( char ) );
	strcpy( node -> var, str );
	
	#define OPER_DEF( cmd, cmd_type, FUNC, operatr, code )		\
	do								\
	{								\
		if( !strcmp( str, #operatr ) )				\
		{							\
			node -> type = operat;				\
			node -> oper = cmd;				\
			return 0;					\
		}							\
	}while( 0 );

	#include "operators.h"
	#undef OPER_DEF
}

struct tree_node_t *num( struct tree_t *tree, double val );

struct tree_node_t *diff( struct tree_t *tree, struct tree_node_t *node, char *dvar )
{
	check_pointer( tree, NULL );
	check_pointer( node, NULL );

	if( isDVAR ) return num( tree, 1 );

	if( ( isNUM ) || ( isVAR ) ) return num( tree, 0 );

	switch( OPERAT )
	{
		#define OPER_DEF( cmd, type, FUNC, operatr, code )	\
		do							\
		{							\
			case cmd:					\
			{						\
				return code;				\
			}						\
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
				#define OPER_DEF( cmd, type, FUNC, operatr, code )	\
				case cmd:						\
				{							\
					fprintf( out, #operatr );			\
					break;						\
				}
	
				#include "operators.h"
				#undef OPER_DEF
			}
	
			break;
		}
	}
}

#define OPERAT_CODE_bin( NAME, symbol )								\
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
}


#define OPERAT_CODE_uno( NAME, symbol )								\
												\
struct tree_node_t *NAME( struct tree_t *tree, struct tree_node_t *a )				\
{												\
	struct element *elem = ( struct element * )calloc( 1, sizeof( struct element ) );	\
	elem -> type = operat;									\
	elem -> oper = symbol;									\
	struct tree_node_t *parent = tree_node_construct( tree, NULL, elem );			\
	tree_set( parent, left, a );								\
	return parent;										\
}

#define OPER_DEF( cmd, type, FUNC, operatr, code ) OPERAT_CODE_##type( FUNC, cmd )

#include "operators.h"

#undef OPER_DEF

#undef OPERAT_CODE_bin
#undef OPERAT_CODE_uno

struct tree_t *start( struct aki_structure *akinator )
{
	check_pointer( akinator, NULL );

	struct tree_node_t *root = diff( akinator -> tree, tree_get_root( akinator -> tree ), akinator -> dvar );
	
	tree_create( struct element, newtree, struct_element, tree_get_elem( root ) );

	Do( tree_set( tree_get_root( newtree ), left, tree_get_next( root, left ) ) );
	Do( tree_set( tree_get_root( newtree ), right, tree_get_next( root, right ) ) );

	optimize_it( newtree );

	return newtree;
}

int optimizer1( struct tree_t *tree, struct tree_node_t **node_ptr )
{
	check_pointer( node_ptr, 0 );
	check_pointer( *node_ptr, 0 );
	check_pointer( tree, 0 );
	
	struct tree_node_t *node = *node_ptr;

	struct element *elem = ( struct element * )tree_get_elem( node );
	
	if( elem -> type == operat )
	{
		struct element *left_elem  = ( struct element * )tree_get_elem( tree_get_next( node, left ) );
		struct element *right_elem = ( struct element * )tree_get_elem( tree_get_next( node, right ) );
	
		if( elem -> oper == sub_cmd )
		{
			if( left_elem -> type == koefficient && left_elem -> value == 0 )
			{
				*node_ptr = tree_node_change( node, MUL( tree, num( tree, -1 ), R ) );

				return 1;
			}
		}	

		if( elem -> oper == add_cmd || elem -> oper == sub_cmd )
		{
			if( left_elem -> type == koefficient && left_elem -> value == 0 )				
			{
				*node_ptr = tree_node_change( node, R );

				return 1;							
			}

			if( right_elem -> type == koefficient && right_elem -> value == 0 )
			{
				*node_ptr = tree_node_change( node, L );

				return 1;
			}
		}

		if( elem -> oper == mul_cmd )
		{
			if( left_elem -> type == koefficient && left_elem -> value == 1 )				
			{					
				*node_ptr = tree_node_change( node, R );

				return 1;

			}
			
			if( right_elem -> type == koefficient && right_elem -> value == 1 )
			{
				*node_ptr = tree_node_change( node, L );

				return 1;
			}

			if( left_elem -> type == koefficient && left_elem -> value == 0 )				
			{					
				*node_ptr = tree_node_change( node, num( tree, 0 ) );

				return 1;
			}

			if( right_elem -> type == koefficient && right_elem -> value == 0 )
			{
				*node_ptr = tree_node_change( node, num( tree, 0 ) );

				return 1;
			}
		}

		if( elem -> oper == div_cmd )
		{
			if( left_elem -> type == koefficient && left_elem -> value == 0 )
			{
				*node_ptr = tree_node_change( node, num( tree, 0 ) );

				return 1;
			}
		}

		if( elem -> oper == pow_cmd )
		{
			if( right_elem -> value == 1 )
			{
				*node_ptr = tree_node_change( node, L );

				return 1;
			}

			if( right_elem -> value == 0 )
			{
				*node_ptr = tree_node_change( node, num( tree, 1 ) );

				return 1;
			}
		}

		if( left_elem -> type == koefficient && right_elem -> type == koefficient )
		{
			switch( elem -> oper )
			{
				#define CASE( cmd, operatr )								\
				case cmd:										\
				{											\
					*node_ptr = tree_node_change( node, num( tree, left_elem -> value operatr right_elem -> value ) );	\
															\
					break;										\
				}

				CASE( add_cmd, + )
				CASE( sub_cmd, - )
				CASE( div_cmd, / )
				CASE( mul_cmd, * )

				#undef CASE
			}

			return 1;
		}
	}

	int kostyl = 0;

	if( tree_get_next( *node_ptr, left ) != NULL )
	{
		struct tree_node_t *nextnode = tree_get_next( *node_ptr, left );
		kostyl += optimizer1( tree, &nextnode );
	}

	if( tree_get_next( *node_ptr, right ) != NULL )
	{
		struct tree_node_t *nextnode = tree_get_next( *node_ptr, right );
		kostyl += optimizer1( tree, &nextnode );
	}

	return kostyl;
}

int optimizer2( struct tree_t *tree, struct tree_node_t *node )
{
	check_pointer( node, 0 );

	struct element *elem = ( struct element * )tree_get_elem( node );
	
	if( elem -> type == operat )
	{
		struct element *left_elem = ( struct element * )tree_get_elem( tree_get_next( node, left ) );
		struct element *right_elem = ( struct element * )tree_get_elem( tree_get_next( node, right ) );

		if( elem -> oper == mul_cmd )
		{
			if( ( left_elem -> type == koefficient && left_elem -> value == 0 ) || ( right_elem -> type == koefficient && right_elem -> value == 0 ) )
			{
				del_branch( tree, node );
	
				return 1;
			}
		}
	}

	if( tree_get_next( node, left ) != NULL )
		optimizer2( tree, tree_get_next( node, left ) );
	
	if( tree_get_next( node, right ) != NULL )
		optimizer2( tree, tree_get_next( node, right ) );
	
	return 0;
}

int optimize_it( struct tree_t *tree )
{
	check_pointer( tree, 0 );

	struct tree_node_t *root = tree_get_root( tree );

	while( optimizer1( tree, &root ) )
		;

	optimizer2( tree, tree_get_root( tree ) );
}

int texer( FILE* doc, struct tree_node_t *node )
{
	check_pointer( doc, 1 );
	check_pointer( node, 1 );

	struct element *elem = ( struct element * )tree_get_elem( node );

	switch( elem -> type )
	{
		case koefficient:
		{
			if( elem -> value < 0 )
				fprintf( doc, "(%lg)", elem -> value );

			else
				fprintf( doc, "{%lg}", elem -> value );

			break;
		}
		
		case variable:
		{
			fprintf( doc, "%s", elem -> var );
			break;
		}

		case operat:
		{
			switch( OPERAT )
			{
				#define OPER_DEF( cmd, cmd_type, FUNC, operatr, code )			\
													\
				case cmd:								\
				{									\
					if( !strcmp( #cmd_type, "bin" ) )				\
					{								\
						if( !strcmp( #operatr, "/" ) )				\
						{							\
							fprintf( doc, "\\frac{" );			\
							texer( doc, tree_get_next( node, left ) );	\
							fprintf( doc, "}{" );				\
							texer( doc, tree_get_next( node, right ) );	\
							fprintf( doc, "}" );				\
													\
							break;						\
						}							\
													\
						if( !strcmp( #operatr, "^" ) )				\
						{							\
							fprintf( doc, "\\left(" );			\
							texer( doc, tree_get_next( node, left ) );	\
							fprintf( doc, "\\right)" );			\
							fprintf( doc, "^" );				\
							texer( doc, tree_get_next( node, right ) );	\
													\
							break;						\
						}							\
													\
						else							\
						{								\
							if( strcmp( #operatr, "*" ) && strcmp( #operatr, "/" ) )	\
								fprintf( doc, "\\left(" );				\
							texer( doc, tree_get_next( node, left ) );			\
							fprintf( doc, #operatr );					\
							texer( doc, tree_get_next( node, right ) );			\
							if( strcmp( #operatr, "*" ) && strcmp( #operatr, "/" ) )	\
								fprintf( doc, "\\right)" );			\
													\
							break;						\
						}							\
					}								\
													\
					if( !strcmp( #cmd_type, "uno" ) )				\
					{								\
						fprintf( doc, "\\"#operatr"\\left(" );			\
						texer( doc, tree_get_next( node, left ) );		\
						fprintf( doc, "\\right)" );				\
													\
						break;							\
					}								\
				}

				#include "operators.h"
				#undef OPER_DEF
			}
			
			break;
		}
	}
}

int tex_it( struct aki_structure *akinator )
{
	check_pointer( akinator, 1 );

	FILE *doc = fopen( "tex", "w" );
	fprintf( doc,	"\\documentclass[14pt,twoside,a4paper]{report}\n"
			"\\begin{document}\n"
			"$$f = \n" );

	Do( texer( doc, tree_get_root( akinator -> tree ) ) );

	fprintf( doc, 	"$$\n"
			"$$\\frac{df}{d%s} = ", akinator -> dvar );

	Do( texer( doc, tree_get_root( akinator -> newtree ) ) );

	fprintf( doc,	"$$\n"
			"\\end{document}\n" );

	fclose( doc );
}
