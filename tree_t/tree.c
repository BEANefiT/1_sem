typedef int elem_t;

struct node_t
{
	elem_t elem;
	int *parent;
	int *left;
	int *right;
};

struct tree_t
{
	int POISON_val;
	size_t size;
	int *root;
};

int tree_construct( struct tree_t *tree, const int POISON )
{
	tree -> POISON_val = POISON;
	tree -> size = 1;
	tree -> root = ( struct node_t * )calloc( 1, sizeof( struct node_t ) );
	tree -> root -> elem = tree -> POISON_val;
	tree -> root -> left = NULL;
	tree -> root -> right = NULL;
	tree -> root -> parent = NULL;
}

int add_right_child( struct tree_t *tree, int *parent, elem_t value )
{
	tree -> size ++;
	parent -> right = ( struct node_t * )calloc( 1, sizeof( struct node_t ) );
	parent -> right -> elem = value;
	parent -> right -> parent = parent;
	parent -> right -> left = NULL;
	parent -> right -> right = NULL;
}

int add_left_child( struct tree_t *tree, int *parent, elem_t value )
{
	tree -> size ++;
	parent -> left = ( struct node_t * )calloc( 1, sizeof( struct node_t ) );
	parent -> left -> elem = value;
	parent -> left -> parent = parent;
	parent -> left -> left = NULL;
	parent -> left -> right = NULL;
}

elem_t
