#define __TREE_CHAR__
typedef char *elem_t;
typedef struct tree_node_t tree_node_t;
typedef struct tree_t tree_t;
enum side_t
{
	left,
	right
};
struct tree_node_t *tree_construct( struct tree_t **tree, elem_t POISON, elem_t elem );
struct tree_node_t *tree_add( struct tree_t *tree, struct tree_node_t *parent, enum side_t side, elem_t elem );
int change_elem( struct tree_t *tree, struct tree_node_t *node, elem_t arg );
int del_branch( struct tree_t *tree, struct tree_node_t *parent );
struct tree_node_t *tree_find( struct tree_node_t *root, elem_t target );
int dumper( struct tree_t *tree );
