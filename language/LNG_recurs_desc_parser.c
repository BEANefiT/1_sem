#include <stdio.h>
#include <stdlib.h>

int analyser_make_tree( struct analyser_t *analyser )
{
	check_pointer( analyser, 1 );

	struct tree_node_t *root = getFunc( analyser );
}

struct tree_node_t *getFunc( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ analyser -> cur_pos ] -> key != 6 )
		return NULL;

	struct tree_node_t *node = 
		tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++] );

	check_syntax( '<' );

	int param_count = 0;

	struct lex_t **params_arr =
		( struct lex_t ** )calloc( LRDP_MAX_PARAM_COUNT, sizeof( struct lex_t * ) );
	check_pointer( params_arr, NULL );

	while
	(
		analyser -> lexems[ analyser -> cur_pos ] -> key != 5
	&&
		*analyser -> lexems[ analyser -> cur_pos ] -> value != '>'
	)
	
	{
		if( param_count == LRDP_MAX_PARAM_COUNT )
		{
			printf( "too many arguments near lexem(%zd)\n", analyser -> cur_pos );
			
			return NULL;
		}

		params_arr[ param_count++ ] = analyser -> lexems[ analyser -> cur_pos++ ];
	}

	check_syntax( '>' );

	params_arr = ( struct lex_t ** )realloc( params_arr, param_count * sizeof( struct lex_t * ) );
	check_pointer( params_arr, NULL );

	struct func_t *func = ( struct func_t * )calloc( 1, sizeof( struct func_t ) );
	check_pointer( func, NULL );

	func -> param_count = param_count;
	func -> params_arr = params_arr;

	struct lex_t *parameters = ( struct lex_t * )calloc( 1, sizeof( struct lex_t ) );
	check_pointer( parameters, NULL );

	parameters -> key = 7;
	parameters -> func = func;

	check_syntax( '[' );

	struct tree_node_t *newnode = getI( analyser );
	check_pointer( func_left, NULL );

	tree_set( node, left, newnode );

	while
	(
		analyser -> lexems[ analyser -> cur_pos ] -> key != 5
	&&
		*analyser -> lexems[ analyser -> cur_pos ] -> value != ']'
	)

	{
		struct tree_node_t *tmp_node = getI( analyser );
		check_poiunter( tmp_node, NULL );

		tree_set( newnode, right, tmp_node );
		
		newnode = tmp_node;
	}
	
	check_syntax( ']' );

	return node;
}

struct tree_node_t *getI( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	struct tree_node_t node = getFunc( analyser );

	if( !node )
		node = getAssn( analyser );

	if( !node )
		node = getKw( analyser );

	return node;
}

struct tree_node_t *getAssn( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ analyser -> cur_pos ] -> key != 4 )
		return NULL;

	struct tree_node_t *var = getVAR( analyser );
	check_pointer( var, NULL );

	struct tree_node_t *node =
		tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );
	check_pointer( node, NULL );

	struct tree_node_t *E = getE( analyser );
	check_pointer( E, NULL );

	tree_set( node, left, var );
	tree_set( var, right, E );
	
	return node;
}

struct tree_node_t *getKw( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ cur_pos ] -> key != 3 )
		return NULL;

	struct tree_node_t *node =
		tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos ] );
	check_pointer( node, NULL );

	switch( *analyser -> lexems[ analyser -> cur_pos++ ] -> value )
	{
		case '1':
		{
			check_syntax( '(' );
			
			struct tree_node_t *cond = getE( analyser );
			check_pointer( cond, NULL );

			check_syntax( ')' );

			break;
		}

		case '2':
		{
			break;
		}

		case '3':
		{
			check_syntax( '(' );
			
			struct tree_node_t *cond = getE( analyser );
			check_pointer( cond, NULL );

			check_syntax( ')' );

			break;
		}
	}
	
	check_syntax( '[' );

	struct tree_node_t *newnode = getI( analyser );
	check_pointer( func_left, NULL );

	tree_set( node, left, newnode );

	while
	(
		analyser -> lexems[ analyser -> cur_pos ] -> key != 5
	&&
		*analyser -> lexems[ analyser -> cur_pos ] -> value != ']'
	)

	{
		struct tree_node_t *tmp_node = getI( analyser );
		check_poiunter( tmp_node, NULL );

		tree_set( newnode, right, tmp_node );
		
		newnode = tmp_node;
	}
	
	check_syntax( ']' );

	return node;
}

struct tree_node_t *getVAR( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	if( analyser -> lexems[ analyser -> cur_pos ] -> key != 4 )
		return NULL;

	return tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );
}

struct tree_node_t *getE( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	struct tree_node_t *node = getT( analyser );
	check_pointer( node, NULL );

	char next_elem = *analyser -> lexems[ analyser -> cur_pos ] -> value;

	while( next_elem == '+' || next_elem == '-' )
	{
		struct tree_node_t *oper = analyser -> lexems[ analyser -> cur_pos++ ];
		check_pointer( oper, NULL );

		struct tree_node_t *node2 = getT( analyser );
		check_pointer( node2, NULL );

		struct tree_node_t *node_cp = tree_copy( analyser -> tree, node );
		check_pointer( node_cp, NULL );

		tree_set( oper, left, node_cp );
		tree_set( oper, right, node2 );

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
		struct tree_node_t *oper = analyser -> lexems[ analyser -> cur_pos++ ];
		check_pointer( oper, NULL );

		struct tree_node_t *node2 = getB( analyser );
		check_pointer( node2, NULL );

		struct tree_node_t *node_cp = tree_copy( analyser -> tree, node );
		check_pointer( node_cp, NULL );

		tree_set( oper, left, node_cp );
		tree_set( oper, right, node2 );

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
		struct tree_node_t *oper = analyser -> lexems[ analyser -> cur_pos++ ];
		check_pointer( oper, NULL );

		struct tree_node_t *node2 = getP( analyser );
		check_pointer( node2, NULL );

		struct tree_node_t *node_cp = tree_copy( analyser -> tree, node );
		check_pointer( node_cp, NULL );

		tree_set( oper, left, node_cp );
		tree_set( oper, right, node2 );

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
		struct tree_node_t *node = getE( analyser );
		check_pointer( node, NULL );

		check_syntax( ')' );

		return node;
	}

	struct tree_node_t *node = getN( analyser );

	if( !node )
		node = getFunc( analyser );

	return node;
}

struct tree_node_t *getN( struct analyser_t *analyser )
{
	check_pointer( analyser, NULL );

	struct tree_node_t *node = getVAR( analyser );

	if( !node )
	{
		if( analyser -> lexems[ analyser -> cur_node ] -> key != 1 )
		{
			printf( "Syntax error: lexems[ %zd ] != val\n", cur_node );

			return NULL;
		}

		node = tree_node_construct( analyser -> tree, NULL, analyser -> lexems[ analyser -> cur_pos++ ] );
	}

	return node;
}
