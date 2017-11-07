#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	int POISON_val;
	size_t size;
	struct tree_node_t *root;
};

#ifdef __TREE_CHAR__
size_t tree_node_sz( elem_t elem )
{
	size_t sz = 3 * sizeof( struct tree_node_t * ) + strlen( elem );
	return sz;
}
#endif

struct tree_node_t *tree_node_construct( struct tree_t *tree, struct tree_node_t *parent, elem_t elem, size_t sz )
{
	struct tree_node_t *node = ( struct tree_node_t * )calloc( 1, sz );
	node -> elem = elem;
	node -> parent = parent;
	node -> left = NULL;
	node -> right = NULL;
	tree -> size++;
	return node;
}

int tree_construct( struct tree_t *tree, const int POISON, elem_t elem )
{
	tree -> POISON_val = POISON;
	tree -> size = 0;
	size_t sz = tree_node_sz( elem );
	tree -> root = tree_node_construct( tree, NULL, elem, sz );
}

struct tree_node_t *tree_add( struct tree_t *tree, struct tree_node_t *parent, enum side_t side, elem_t elem )
{
	size_t sz = tree_node_sz( elem );
	if( side == left )
		parent -> left = tree_node_construct( tree, parent, elem, sz );
	if( side == right )
		parent -> right = tree_node_construct( tree, parent, elem, sz );

}

int change_elem( struct tree_t *tree, struct tree_node_t *node, elem_t arg )
{
	node -> elem = arg;
}

int del_branch( struct tree_t *tree, struct tree_node_t *parent )
{
	if( parent -> left != NULL )
		del_branch( tree, parent -> left );
	if( parent -> right != NULL )
		del_branch( tree, parent -> right );
	free( parent );
	tree -> size --;
}

struct tree_node_t *tree_find( struct tree_node_t *root, elem_t target )
{
	if( root -> elem == target )
		return root;
	if( root -> left != NULL )
		root = tree_find( root -> left, target );
	if( root -> right != NULL )
		root = tree_find( root -> right, target );
}


// man exec
// f(p)ork()


/*int dump_node( FILE *dump, struct tree_node_t *node, int num )
{
	fprintf( dump, "Node%d [shape = record, label = \"%s\n%p | ",
		 num, node -> elem, node );
	if( node -> left != NULL )
		fprintf( dump, "{ left = %p\n%s ", node -> left, node -> left -> elem );
	if( node -> left == NULL )
		fprintf( dump, "{ left = NULL " );
	if( node -> right != NULL )
		fprintf( dump, "| right = %p\n%s }", node -> right, node -> right -> elem );
	if( node -> right == NULL )
		fprintf( dump, "| right = NULL }" );
	fprintf( dump, "\"]\n" );
	if( node -> left != NULL )
		dump_node( dump, node -> left, num++ );
	if( node -> right != NULL )
		dump_node( dump, node -> right, num =++ );
}

int dumper( struct tree_t *tree )
{
	FILE *dump = fopen( "dump", "w" );
	fprintf( dump, "digraph dump\n"
			"{\n"
	int num = 0;
	if( root != NULL )
		dump_node( dump, root, num++ );
	for( int i = 0; i < num; i++ )
	{
		
	}
}
*/
