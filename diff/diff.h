struct aki_structure
{
	struct tree_t *tree;
	struct tree_t *newtree;
	char *src;
	size_t src_sz;
	size_t src_cur;
};

enum type_t
{
	koefficient,
	variable,
	operat
};

typedef struct element element;

size_t 	src_sz( FILE *src );
char*	getsrc( char *src_name, struct aki_structure *akinator );
int 	aki_constr( struct aki_structure *akinator );
int 	make_tree( struct aki_structure *akinator );
int 	start( struct aki_structure *akinator );
//int 	base_edit( struct aki_structure *akinator, char *base_name );

int print_struct_element( FILE *out, void *elem );
int cmp_struct_element( void *elem1, void *elem2 ){}

struct tree_node_t *ADD( struct tree_t *tree, struct tree_node_t *a, struct tree_node_t *b );
struct tree_node_t *SUB( struct tree_t *tree, struct tree_node_t *a, struct tree_node_t *b );
struct tree_node_t *MUL( struct tree_t *tree, struct tree_node_t *a, struct tree_node_t *b );
struct tree_node_t *DIV( struct tree_t *tree, struct tree_node_t *a, struct tree_node_t *b );

#define L tree_copy( tree, tree_get_next( node, left ) )

#define R tree_copy( tree, tree_get_next( node, right ) )

#define dL diff( tree, tree_get_next( node, left ) )

#define dR diff( tree, tree_get_next( node, right ) )

#define isNUM ( ( struct element * )tree_get_elem( node ) ) -> type == koefficient

#define isVAR ( ( struct element * )tree_get_elem( node ) ) -> type == variable

#define OPERAT ( ( struct element * )tree_get_elem( node ) ) -> oper
