#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
struct node_t
{
	elem_t elem;
	struct node_t *parent;
	struct node_t *left;
	struct node_t *right;
};

struct tree_t
{
	int POISON_val;
	size_t size;
	size_t capacity;
	struct node_t *root;
};

int tree_construct( struct tree_t *tree, const int POISON )
{
	tree -> POISON_val = POISON;
	tree -> size = 1;
	tree -> capacity = 0;
	tree -> root = ( struct node_t * )calloc( 1, sizeof( struct node_t ) );
	tree -> root -> elem = tree -> POISON_val;
	tree -> root -> left = NULL;
	tree -> root -> right = NULL;
	tree -> root -> parent = NULL;
}

int add_right_node( struct tree_t *tree, struct node_t *parent )
{
	tree -> size ++;
	parent -> right = ( struct node_t * )calloc( 1, sizeof( struct node_t ) );
	parent -> right -> elem = tree -> POISON_val;
	parent -> right -> parent = parent;
	parent -> right -> left = NULL;
	parent -> right -> right = NULL;
}

int add_left_node( struct tree_t *tree, struct node_t *parent )
{
	tree -> size ++;
	parent -> left = ( struct node_t * )calloc( 1, sizeof( struct node_t ) );
	parent -> left -> elem = tree -> POISON_val;
	parent -> left -> parent = parent;
	parent -> left -> left = NULL;
	parent -> left -> right = NULL;
}

int change_elem( struct tree_t *tree, struct node_t *node, elem_t arg )
{
	tree -> capacity ++;
	node -> elem = arg;
}

int del_branch( struct tree_t *tree, struct node_t *parent )
{
	if( parent -> left != NULL )
		del_branch( tree, parent -> left );
	if( parent -> right != NULL )
		del_branch( tree, parent -> right );
	free( parent );
	tree -> capacity --;
	tree -> size --;
}

struct node_t *find_elem( struct node_t *root, elem_t target )
{
	if( root -> elem == target )
		return root;
	if( root -> left != NULL )
		find_elem( root -> left, target );
	if( root -> right != NULL )
		find_elem( root -> right, target );
}
