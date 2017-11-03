typedef int elem_t;
typedef struct node_t node_t;
typedef struct tree_t tree_t;
int tree_construct( struct tree_t *tree, const int POISON );
int add_right_node( struct tree_t *tree, struct node_t *parent );
int add_left_node( struct tree_t *tree, struct node_t *parent );
int change_elem( struct tree_t *tree, struct node_t *node, elem_t arg );
int del_branch( struct tree_t *tree, struct node_t *parent );
struct node_t *find_elem( struct node_t *root, elem_t target );
