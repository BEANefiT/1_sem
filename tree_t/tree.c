#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <log.h>
#include "tree.h"
struct tree_node_t
{
	void *elem;
	struct tree_node_t *parent;
	struct tree_node_t *left;
	struct tree_node_t *right;
};

struct tree_t
{
	print_function printer;
	cmp_function comparer;
	int elem_sz;
	size_t size;
	struct tree_node_t *root;

};

struct tree_node_t *tree_node_construct( struct tree_t *tree, struct tree_node_t *parent, void *elem )
{
	if( tree == NULL )
	{
		print_log( "ERROR: TRYING TO NODE_CONSTR FOR NO TREE\n" );
		return NULL;
	}
	if( parent == NULL )
	{
		print_log( "ERROR: TRYING TO NODE_CONSTR FOR NO PARENT\n" );
	}
	struct tree_node_t *node = ( struct tree_node_t * )calloc( 1, sizeof( struct tree_node_t ) );
	if( node == NULL )
	{
		print_log( "ERROR: CANT ALLOCATE THE MEMORY\n" );
		return NULL;
	}
	node -> elem = calloc( 1, tree -> elem_sz );
	memcpy( node -> elem, elem, tree -> elem_sz );
	node -> parent = parent;
	node -> left = NULL;
	node -> right = NULL;
	tree -> size++;
	return node;
}

struct tree_t *tree_construct( int sz, print_function print_f, cmp_function cmp_f, void *POISON )
{
	struct tree_t *tree = ( struct tree_t * )calloc( 1, sizeof( struct tree_t ) );
	tree -> printer = print_f;
	tree -> comparer = cmp_f;
	tree -> elem_sz = sz;
	tree -> size = 0;
	tree -> root = tree_node_construct( tree, NULL, POISON );
	return tree;
}

struct tree_node_t *tree_add( struct tree_t *tree, struct tree_node_t *parent, enum side_t side, void *elem )
{
	check_pointer( tree, NULL );
	check_pointer( parent, NULL );
	if( side != left && side != right )
	{
		print_log( "ERROR: FALSE SIDE\n" );
		error = UNEXPECTED;
		return NULL;
	}
	if( side == left )
	{
		parent -> left = tree_node_construct( tree, parent, elem );
		return parent -> left;
	}
	if( side == right )
	{
		parent -> right = tree_node_construct( tree, parent, elem );
		return parent -> right;
	}

}

int change_elem( struct tree_t *tree, struct tree_node_t *node, void *arg )
{
	check_pointer( tree, 1 );
	check_pointer( node, 1 );
	check_pointer( arg, 1 );
	memcpy( node -> elem, arg, tree -> elem_sz );
}

int del_branch( struct tree_t *tree, struct tree_node_t *parent )
{
	check_pointer( tree, 0 );
	check_pointer( parent, 0 );
	if( parent -> parent -> left == parent )
		parent -> parent -> left = NULL;
	if( parent -> parent -> right == parent )
		parent -> parent -> right = NULL;
	if( parent -> left != NULL )
		del_branch( tree, parent -> left );
	if( parent -> right != NULL )
		del_branch( tree, parent -> right );
	free( parent );
	parent = NULL;
	tree -> size --;
}

struct tree_node_t *tree_find( struct tree_t *tree, struct tree_node_t *root, void *target )
{
	check_pointer( root, NULL );
	if( !tree -> comparer( root -> elem, target ) )
	{
		return root;
	}
	if( root -> left != NULL )
	{
		struct tree_node_t *newroot = tree_find( tree, root -> left, target );
		if( newroot != NULL )
			return newroot;
	}
	if( root -> right != NULL )
	{
		struct tree_node_t *newroot = tree_find( tree, root -> right, target );
		if( newroot != NULL )
			return newroot;
	}
	return NULL;
}

void *tree_get_elem( struct tree_node_t *node )
{
	check_pointer( node, NULL );
	return node -> elem;
}

struct tree_node_t *tree_get_next( struct tree_node_t *node, enum side_t side )
{
	check_pointer( node, NULL );
	if( side == left )
		return node -> left;
	if( side == right )
		return node -> right;
	return NULL;
}

struct tree_node_t *tree_get_parent( struct tree_node_t *node )
{
	check_pointer( node, NULL );
	return node -> parent;
}

struct tree_node_t *tree_get_root( struct tree_t *tree )
{
	check_pointer( tree, NULL );
	return tree -> root;
}

int dump_node( FILE *dump, struct tree_t *tree, struct tree_node_t *node, struct tree_node_t *parent )
{


	fprintf( dump, "Node%p [shape = record, label = \"{ %p | ", node, node );
	tree -> printer( dump, node -> elem );
	fprintf( dump, " } | " );
	if( node -> left != NULL )
		fprintf( dump, "left = %p ", node -> left );
	if( node -> left == NULL )
		fprintf( dump, "left = NULL " );
	if( node -> right != NULL )
		fprintf( dump, "| right = %p ", node -> right );
	if( node -> right == NULL )
		fprintf( dump, "| right = NULL" );
	fprintf( dump, "\"]\n" );
	if( node -> left != NULL )
		dump_node( dump, tree, node -> left, node );
	if( node -> right != NULL )
		dump_node( dump, tree, node -> right, node );
	if( parent != NULL )
		fprintf( dump, "Node%p -> Node%p\n", parent, node );
}

int dumper( struct tree_t *tree )
{
	check_pointer( tree, 0 );
	FILE *dump = fopen( "dump", "w" );
	fprintf( dump, "digraph dump\n"
			"{\n" );
	if( tree -> root != NULL )
		dump_node( dump, tree, tree -> root, NULL );
	fprintf( dump, "}" );
}

int tree_print( struct tree_t *tree, FILE *out, void *elem )
{
	tree -> printer( out, elem );
}

#define default_func( tp, type )			\
int print_##type( FILE *out, void *elem )		\
{							\
	fprintf( out, "%"#tp, *( type * )elem );	\
}							\
int cmp_##type( void *elem1, void *elem2 )		\
{							\
	return *( type * )elem1 - *( type * )elem2;	\
}

default_func( d, int );
default_func( lg, double );
default_func( g, float );
default_func( zd, size_t );
default_func( c, char );

int print_string( FILE *out, void *elem )
{
	fprintf( out, "%s", *( char ** )elem );
}
int cmp_string( void *elem1, void *elem2 )
{
	return strcmp( *( char ** )elem1, *( char ** )elem2 );
}

#undef default_func
