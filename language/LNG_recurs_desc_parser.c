#include <stdio.h>
#include <stdlib.h>
#include "LNG_recurs_desc_parser.h"

int analyser_make_tree( struct analyser_t *analyser )
{
	check_pointer( analyser, 1 );

	analyser -> tree = tree_constr( sizeof( struct lex_t ), print_lexem, cmp_lexem, NULL );

	struct tree_node_t *root = getFunc( analyser );

	struct tree_node_t *tmp_node = root;
	
	while( analyser -> cur_pos < analyser -> lex_num )
	{
		struct tree_node_t *newnode = getFunc( analyser );

		tree_set( tmp_node, right, newnode );

		tmp_node = newnode;
	}

	analyser -> tree = tree_constr( sizeof( struct lex_t ), print_lexem, cmp_lexem, root );

	return 0;
}

struct tree_node_t *getFunc( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ analyser -> cur_pos ] -> key != 6 )
		return NULL;

	struct lex_t *func_lexem = analyser -> lexems[ analyser -> cur_pos++ ];
	check_pointer( func_lexem, NULL );

	struct func_t *function = ( struct func_t * )calloc( 1, sizeof( struct func_t ) );
	check_pointer( function, NULL );

	function -> name = func_lexem -> value;

	func_lexem -> func = function;

	check_syntax( '{' );

	function -> param_count = 0;

	function -> params_arr =
		( struct lex_t ** )calloc( LRDP_MAX_PARAM_COUNT, sizeof( struct lex_t * ) );
	check_pointer( function -> params_arr, NULL );

	while
	(
		analyser -> lexems[ analyser -> cur_pos ] -> key != 5
	&&
		*analyser -> lexems[ analyser -> cur_pos ] -> value != '}'
	)
	
	{
		if( function -> param_count == LRDP_MAX_PARAM_COUNT )
		{
			printf( "too many arguments near lexem(%zd)\n", analyser -> cur_pos );
			
			return NULL;
		}
		function -> params_arr[ function -> param_count++ ] = analyser -> lexems[ analyser -> cur_pos++ ];
	}

	check_syntax( '}' );

	if( function -> param_count != 0 )
	{
		function -> params_arr =
			( struct lex_t ** )realloc( function -> params_arr, function -> param_count * sizeof( struct lex_t * ) );
		check_pointer( function -> params_arr, NULL );
	}
	
	else
	{
		free( function -> params_arr );

		function -> params_arr = NULL;
	}

	struct tree_node_t *node = tree_node_construct( analyser -> tree, NULL, func_lexem );
	check_pointer( node, NULL );

	if( *analyser -> lexems[ analyser -> cur_pos ] -> value != '[' )
	{
		function -> mode = call;

		return node;
	}

	check_syntax( '[' );

	function -> mode = decl;
	
	struct tree_node_t *newnode = getI( analyser );
	check_pointer( newnode, NULL );

	tree_set( node, left, newnode );

	while
	(
		analyser -> lexems[ analyser -> cur_pos ] -> key != 5
	&&
		*analyser -> lexems[ analyser -> cur_pos ] -> value != ']'
	)

	{
		struct tree_node_t *tmp_node = getI( analyser );
		check_pointer( tmp_node, NULL );

		tree_set( newnode, right, tmp_node );
		
		newnode = tmp_node;
	}
	
	check_syntax( ']' );

	return node;
}

struct tree_node_t *getI( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	struct tree_node_t *node = getFunc( analyser );

	if( !node )
		node = getAssn( analyser );

	if( !node )
		node = getRet( analyser );

	if( !node )
		node = getOut( analyser );

	if( !node )
		node = getIn( analyser );

	if( !node )
		node = getSqrt( analyser );

	if( !node )
		node = getKw( analyser );

	if( !node )
		node = getDclr( analyser );

	return node;
}

struct tree_node_t *getAssn( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( *analyser -> lexems[ analyser -> cur_pos + 1 ] -> value != '=' )
		return NULL;

	struct tree_node_t *var = getVar( analyser );
	check_pointer( var, NULL );

	struct tree_node_t *node =
		tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );
	check_pointer( node, NULL );

	struct tree_node_t *E = getE( analyser );
	check_pointer( E, NULL );

	tree_set( node, left, var );
	tree_set( var, right, E );

	check_syntax( ';' );
	
	return node;
}

struct tree_node_t *getRet( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ analyser -> cur_pos ] -> key != 9 )
		return NULL;

	struct tree_node_t *node =
		tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );

	if( *analyser -> lexems[ analyser -> cur_pos ] -> value != ';' )
	{
		struct tree_node_t *arg = getE( analyser );

		if( arg )
			tree_set( node, left, arg );
	}

	check_syntax( ';' );

	return node;
}

struct tree_node_t *getOut( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ analyser -> cur_pos ] -> key != 11 )
		return NULL;

	struct tree_node_t *node =
		tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );

	struct tree_node_t *arg = getE( analyser );
	check_pointer( arg, NULL );

	tree_set( node, left, arg );

	check_syntax( ';' );

	return node;
}

struct tree_node_t *getIn( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ analyser -> cur_pos ] -> key != 12 )
		return NULL;

	struct tree_node_t *node =
		tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );

	struct tree_node_t *arg = getE( analyser );
	check_pointer( arg, NULL );

	tree_set( node, left, arg );

	check_syntax( ';' );

	return node;
}

struct tree_node_t *getSqrt( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ analyser -> cur_pos ] -> key != 13 )
		return NULL;

	struct tree_node_t *node =
		tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );

	check_syntax( '{' );

	struct tree_node_t *arg = getE( analyser );
	check_pointer( arg, NULL );

	tree_set( node, left, arg );

	check_syntax( '}' );

	return node;
}

struct tree_node_t *getDclr( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ analyser -> cur_pos ] -> key != 8 )
		return NULL;

	struct tree_node_t *node =
		tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );

	struct tree_node_t *var = getVar( analyser );
	check_pointer( var, NULL );

	tree_set( node, left, var );

	check_syntax( ';' );

	return node;
}

struct tree_node_t *getKw( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ analyser -> cur_pos ] -> key != 3 )
		return NULL;

	struct tree_node_t *node =
		tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos ] );
	check_pointer( node, NULL );

	struct lex_t *cond_num = ( struct lex_t * )calloc( 1, sizeof( struct lex_t ) );
	check_pointer( cond_num, NULL );

	cond_num -> key = 7;

	#define DEF_KW( kw, num, conds )						\
		case num + '0':								\
		{									\
			*cond_num -> value = conds + '0';				\
											\
			struct tree_node_t *conds_node =				\
				tree_node_construct( analyser -> tree, node, cond_num );\
											\
			tree_set( node, left, conds_node );				\
											\
			if( conds == 0 )						\
				break;							\
											\
			check_syntax( '(' );						\
											\
			if( conds == 1 )						\
			{								\
				struct tree_node_t *tmp_node = getE( analyser );	\
				tree_set( conds_node, left, tmp_node );			\
											\
				check_syntax( ')' );					\
											\
				break;							\
			}								\
											\
			struct tree_node_t *tmp_node = getI( analyser );		\
			check_pointer( tmp_node, NULL );				\
											\
			tree_set( conds_node, left, tmp_node );				\
											\
			struct tree_node_t *cond = getE( analyser );			\
			check_pointer( cond, NULL );					\
											\
			tree_set( tmp_node, right, cond );				\
											\
			tmp_node = getI( analyser );					\
			check_pointer( tmp_node, NULL );				\
											\
			tree_set( cond, right, tmp_node );				\
											\
			check_syntax( ')' );						\
											\
			break;								\
		}

	switch( *analyser -> lexems[ analyser -> cur_pos++ ] -> value )
	{
		#include "kwrds.h"
	}

	#undef DEF_KW

	check_syntax( '[' );

	struct tree_node_t *newnode = getI( analyser );
	check_pointer( newnode, NULL );

	tree_set( tree_get_next( node, left ), right, newnode );

	while
	(
		analyser -> lexems[ analyser -> cur_pos ] -> key != 5
	&&
		*analyser -> lexems[ analyser -> cur_pos ] -> value != ']'
	)

	{
		struct tree_node_t *tmp_node = getI( analyser );
		check_pointer( tmp_node, NULL );

		tree_set( newnode, right, tmp_node );
		
		newnode = tmp_node;
	}
	
	check_syntax( ']' );

	return node;
}

struct tree_node_t *getVar( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ analyser -> cur_pos ] -> key != 4 )
		return NULL;

	return tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );
}

struct tree_node_t *getE( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	struct tree_node_t *node = getM( analyser );
	check_pointer( node, NULL );

	char next_elem = *analyser -> lexems[ analyser -> cur_pos ] -> value;

	while( next_elem == '<' || next_elem == '>' || next_elem == '?' )
	{
		struct tree_node_t *oper =
			tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );
		check_pointer( oper, NULL );

		struct tree_node_t *node2 = getM( analyser );
		check_pointer( node2, NULL );

		struct tree_node_t *node_cp = tree_copy( analyser -> tree, node );
		check_pointer( node_cp, NULL );

		tree_set( oper, left, node_cp );
		tree_set( node_cp, right, node2 );

		node = tree_node_change( node, oper );

		next_elem = *analyser -> lexems[ analyser -> cur_pos ] -> value;
	}

	return node;
}

struct tree_node_t *getM( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	struct tree_node_t *node = getT( analyser );
	check_pointer( node, NULL );

	char next_elem = *analyser -> lexems[ analyser -> cur_pos ] -> value;

	while( next_elem == '+' || next_elem == '-' )
	{
		struct tree_node_t *oper =
			tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );
		check_pointer( oper, NULL );

		struct tree_node_t *node2 = getT( analyser );
		check_pointer( node2, NULL );

		struct tree_node_t *node_cp = tree_copy( analyser -> tree, node );
		check_pointer( node_cp, NULL );

		tree_set( oper, left, node_cp );
		tree_set( node_cp, right, node2 );

		node = tree_node_change( node, oper );

		next_elem = *analyser -> lexems[ analyser -> cur_pos ] -> value;
	}

	return node;
}

struct tree_node_t *getT( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	struct tree_node_t *node = getB( analyser );
	check_pointer( node, NULL );

	char next_elem = *analyser -> lexems[ analyser -> cur_pos ] -> value;

	while( next_elem == '*' || next_elem == '/' )
	{
		struct tree_node_t *oper =
			tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );
		check_pointer( oper, NULL );

		struct tree_node_t *node2 = getB( analyser );
		check_pointer( node2, NULL );

		struct tree_node_t *node_cp = tree_copy( analyser -> tree, node );
		check_pointer( node_cp, NULL );

		tree_set( oper, left, node_cp );
		tree_set( node_cp, right, node2 );

		node = tree_node_change( node, oper );

		next_elem = *analyser -> lexems[ analyser -> cur_pos ] -> value;
	}

	return node;
}

struct tree_node_t *getB( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	struct tree_node_t *node = getP( analyser );
	check_pointer( node, NULL );

	char next_elem = *analyser -> lexems[ analyser -> cur_pos ] -> value;

	while( next_elem == '^' )
	{
		struct tree_node_t *oper =
			tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );
		check_pointer( oper, NULL );

		struct tree_node_t *node2 = getP( analyser );
		check_pointer( node2, NULL );

		struct tree_node_t *node_cp = tree_copy( analyser -> tree, node );
		check_pointer( node_cp, NULL );

		tree_set( oper, left, node_cp );
		tree_set( node_cp, right, node2 );

		node = tree_node_change( node, oper );

		next_elem = *analyser -> lexems[ analyser -> cur_pos ] -> value;
	}

	return node;
}

struct tree_node_t *getP( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( *analyser -> lexems[ analyser -> cur_pos ] -> value == '(' )
	{
		analyser -> cur_pos++;
		struct tree_node_t *node = getE( analyser );
		check_pointer( node, NULL );

		check_syntax( ')' );

		return node;
	}

	struct tree_node_t *node = getN( analyser );

	if( !node )
		node = getI( analyser );

	return node;
}

struct tree_node_t *getN( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	struct tree_node_t *node = getVar( analyser );

	if( !node )
	{
		if( analyser -> lexems[ analyser -> cur_pos ] -> key != 1 )
			return NULL;

		node = tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );
	}

	return node;
}

int print_lexem( FILE *out, void *elem )
{
	struct lex_t *lexem = ( struct lex_t * )elem;

	switch( lexem -> key )
	{
		case 1:
		{
			fprintf( out, "key - 'val' | '%lg'", lexem -> koeff );

			break;
		}

		case 2:
		{
			if( *lexem -> value == '>' || *lexem -> value == '<' )
			{
				fprintf( out, "key - 'oper' | \\\"\\%c\\\" ", *lexem -> value );

				break;
			}

			fprintf( out, "key - 'oper' | '%c'", *lexem -> value );

			break;
		}

		case 3:
		{
			fprintf( out, "key - 'kw' | " );

			#define DEF_KW( name, num, conds )		\
				case num + '0':				\
				{					\
					fprintf( out, "'"#name"'" );	\
									\
					break;				\
				}

			switch( *lexem -> value )
			{
				#include "kwrds.h"
			}

			#undef DEF_KW

			break;
		}

		case 4:
		{
			fprintf( out, "key - 'var' | '%s'", lexem -> value );

			break;
		}

		case 5:
		{
			fprintf( out, "key - 'br' | '%c'", *lexem -> value );

			break;
		}

		case 6:
		{
			struct func_t *function = lexem -> func;

			fprintf( out, "key - 'func' | '%s' | %d params | mode = ", function -> name, function -> param_count );

			if( function -> mode == decl )
				fprintf( out, "decl" );

			if( function -> mode == call )
				fprintf( out, "call" );

			break;
		}

		case 7:
		{
			fprintf( out, "%d cond(s)", *lexem -> value - '0' );

			break;
		}

		case 8:
		{
			fprintf( out, "declaration" );

			break;
		}

		case 9:
		{
			fprintf( out, "return" );

			break;
		}

		case 11:
		{
			fprintf( out, "out" );

			break;
		}

		case 12:
		{
			fprintf( out, "in" );

			break;
		}
	}
}

int cmp_lexem( void *elem1, void *elem2 ){}
