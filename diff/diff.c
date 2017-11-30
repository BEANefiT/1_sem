#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	Do( akinator.src = getsrc( argv[ 1 ], &akinator ) );
	
	if( akinator.src == NULL )
	{
		printf( "ERROR: '%s' no such file or directory\n", argv[ 1 ] );
		return 0;
	}

	Do( make_tree( &akinator ) );

	Do( akinator.newtree = start( akinator.tree ) );

	Do( dumper( akinator.tree ) );

	tex_it( &akinator );
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
	struct element *root_elem = NULL;
	if( src_cur_delta != 0 )
		{
			root_elem = ( struct element * )calloc( 1, sizeof( struct element ) );
			root_elem -> value = koeff;
			root_elem -> type = koefficient;
		}
	if( src_cur_delta == 0 )
		{
			sscanf( akinator -> src + akinator -> src_cur, "%*1[\'\"]%[^\'\"]%*1[\'\"]%n", str, &src_cur_delta );
			root_elem = ( struct element * )calloc( 1, sizeof( struct element ) );
			type_recognize( root_elem, str );
		}
	akinator -> src_cur += src_cur_delta;
	tree_create( struct element, tree, struct_element, root_elem );
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

#undef OPERAT_CODE_BIN
#undef OPERAT_CODE_UNO

struct tree_t *start( struct tree_t *tree )
{
	check_pointer( tree, NULL );

	struct tree_node_t *root = diff( tree, tree_get_root( tree ) );
	
	tree_create( struct element, newtree, struct_element, tree_get_elem( root ) );

	Do( tree_set( tree_get_root( newtree ), left, tree_get_next( root, left ) ) );
	Do( tree_set( tree_get_root( newtree ), right, tree_get_next( root, right ) ) );

	return newtree;
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
						fprintf( doc, #operatr"\\left(" );			\
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
			"function is:\n"
			"$$" );

	Do( texer( doc, tree_get_root( akinator -> tree ) ) );

	fprintf( doc, 	"$$\n"
			"derivative is:\n"
			"$$" );

	Do( texer( doc, tree_get_root( akinator -> newtree ) ) );

	fprintf( doc,	"$$\n"
			"\\end{document}\n" );

	fclose( doc );
}
