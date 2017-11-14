#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../log/log.h"
#include "tree.h"
struct tree_node_t
{
	elem_t elem;
	struct tree_node_t *parent;
	struct tree_node_t *left;
	struct tree_node_t *right;
};

struct tree_t
{
	size_t size;
	struct tree_node_t *root;
};

#ifdef __TREE_CHAR__
size_t tree_node_sz( elem_t elem )
{
	size_t sz = 3 * sizeof( struct tree_node_t * ) + strlen( elem );
	return sz;
}
#else
size_t tree_node_sz( elem_t elem )
{
	size_t sz = 3 * sizeof( struct tree_node_t * ) + sizeof( elem_t );
	return sz;
}
#endif /*__TREE_CHAR__*/

struct tree_node_t *tree_node_construct( struct tree_t *tree, struct tree_node_t *parent, elem_t elem, size_t sz )
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
	struct tree_node_t *node = ( struct tree_node_t * )calloc( 1, sz );
	if( node == NULL )
	{
		print_log( "ERROR: CANT ALLOCATE THE MEMORY\n" );
		return NULL;
	}
	node -> elem = elem;
	node -> parent = parent;
	node -> left = NULL;
	node -> right = NULL;
	tree -> size++;
	return node;
}

struct tree_node_t *tree_construct( struct tree_t **tree, elem_t elem )
{
	check_pointer( tree, NULL );
	size_t root_sz = tree_node_sz( elem );
	*tree = ( struct tree_t * )calloc( 1, root_sz );
	if( *tree == NULL )
	{
		print_log( "ERROR: CANT ALLOCATE THE MEMORY FOR TREE_CONSTR\n" );
		error = BAD_ALLOC;
		return NULL;
	}
	( *tree ) -> size = 0;
	( *tree ) -> root = tree_node_construct( *tree, NULL, elem, root_sz );
	return ( *tree ) -> root;
}

struct tree_node_t *tree_add( struct tree_t *tree, struct tree_node_t *parent, enum side_t side, elem_t elem )
{
	check_pointer( tree, NULL );
	check_pointer( parent, NULL );
	if( side != left && side != right )
	{
		print_log( "ERROR: FALSE SIDE\n" );
		error = UNEXPECTED;
		return NULL;
	}
	size_t sz = tree_node_sz( elem );
	if( side == left )
	{
		parent -> left = tree_node_construct( tree, parent, elem, sz );
		return parent -> left;
	}
	if( side == right )
	{
		parent -> right = tree_node_construct( tree, parent, elem, sz );
		return parent -> right;
	}

}

int change_elem( struct tree_node_t *node, elem_t arg )
{
	check_pointer( node, 0 );
	node -> elem = arg;
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

struct tree_node_t *tree_find( struct tree_node_t *root, elem_t target )
{
	check_pointer( root, NULL );
	if( root -> elem == target )
		return root;
	if( root -> left != NULL )
	{
		struct tree_node_t *newroot = root;
		newroot = tree_find( newroot -> left, target );
		if( newroot != NULL )
			return newroot;
	}
	if( root -> right != NULL )
	{
		struct tree_node_t *newroot = root;
		newroot = tree_find( newroot -> right, target );
		if( newroot != NULL )
			return newroot;
	}
	return NULL;
}

elem_t tree_get_elem( struct tree_node_t *node )
{
	if( node != NULL )
		return node -> elem;
	else
	{
		print_log( "ERROR: *node = NULL\n" );
		error = BAD_ARG;
	}
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

// man exec
// f(p)ork()

int dump_node( FILE *dump, struct tree_node_t *node, struct tree_node_t *parent )
{


	fprintf( dump, "Node%p [shape = record, label = \"{ %p | '%s' } | ",
		 node, node,  node -> elem );
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
		dump_node( dump, node -> left, node );
	if( node -> right != NULL )
		dump_node( dump, node -> right, node );
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
		dump_node( dump, tree -> root, NULL );
	fprintf( dump, "}" );
}
