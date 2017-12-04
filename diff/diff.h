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

struct tree_t *	start( struct tree_t *tree );
size_t 		src_sz( FILE *src );
char*		getsrc( char *src_name, struct aki_structure *akinator );
int 		aki_constr( struct aki_structure *akinator );
int 		make_tree( struct aki_structure *akinator );
int		tex_it( struct aki_structure *akinator );
int 		optimize_it( struct tree_t *tree );

int print_struct_element( FILE *out, void *elem );
int cmp_struct_element( void *elem1, void *elem2 ){}

struct tree_node_t *ADD( struct tree_t *tree, struct tree_node_t *a, struct tree_node_t *b );
struct tree_node_t *SUB( struct tree_t *tree, struct tree_node_t *a, struct tree_node_t *b );
struct tree_node_t *MUL( struct tree_t *tree, struct tree_node_t *a, struct tree_node_t *b );
struct tree_node_t *DIV( struct tree_t *tree, struct tree_node_t *a, struct tree_node_t *b );


#define OPERAT_CODE_bin( NAME, symbol )	\
struct tree_node_t *NAME( struct tree_t *tree, struct tree_node_t *a, struct tree_node_t *b );

#define OPERAT_CODE_uno( NAME, symbol )	\
struct tree_node_t *NAME( struct tree_t *tree, struct tree_node_t *a );

#define OPER_DEF( cmd, type, FUNC, operatr, code ) OPERAT_CODE_##type( FUNC, cmd )

#include "operators.h"

#undef OPER_DEF

#undef OPERAT_CODE_bin
#undef OPERAT_CODE_uno


#define L tree_copy( tree, tree_get_next( node, left ) )

#define R tree_copy( tree, tree_get_next( node, right ) )

#define dL diff( tree, tree_get_next( node, left ) )

#define dR diff( tree, tree_get_next( node, right ) )

#define isNUM ( ( struct element * )tree_get_elem( node ) ) -> type == koefficient

#define isVAR ( ( struct element * )tree_get_elem( node ) ) -> type == variable

#define OPERAT ( ( struct element * )tree_get_elem( node ) ) -> oper
