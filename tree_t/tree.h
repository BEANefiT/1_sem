#ifndef __TREE_H__
#define __TREE_H__
typedef struct tree_node_t tree_node_t;
typedef struct tree_t tree_t;
typedef int ( *print_function )( FILE *, void * );
typedef int ( *cmp_function )( void *, void * );

#define tree_create( type, name, func, root_elem )						\
struct tree_t *name = tree_construct( sizeof( type ), print_##func, cmp_##func, root_elem );	\
if( name == NULL )										\
	print_log( "ERROR: CANT CREATE TREE\n" );

enum side_t
{
	left,
	right
};

struct tree_t* 		tree_construct( int sz, print_function func_f, cmp_function cmp_f, void *root_elem );

struct tree_node_t*	tree_node_construct( struct tree_t *tree, struct tree_node_t *parent, void *elem );
struct tree_node_t* 	tree_add( struct tree_t *tree, struct tree_node_t *parent, enum side_t side, void *elem );
struct tree_node_t* 	tree_find( struct tree_t *tree, struct tree_node_t *root, void *target );
struct tree_node_t*	tree_get_next( struct tree_node_t *node, enum side_t side );
struct tree_node_t*	tree_get_parent( struct tree_node_t *node );
struct tree_node_t*	tree_get_root( struct tree_t *tree );
struct tree_node_t*	tree_get_left( struct tree_t *tree );
struct tree_node_t*	tree_copy_node( struct tree_node_t *node );
struct tree_node_t*	tree_copy( struct tree_t *tree, struct tree_node_t *node );
struct tree_node_t*	tree_node_change( struct tree_node_t *node, struct tree_node_t *newnode );

void*			tree_get_elem( struct tree_node_t *node );

int 			tree_print( struct tree_t *tree, FILE *out, void *elem );
int 			change_elem( struct tree_t *tree, struct tree_node_t *node, void *arg );
int 			dumper( struct tree_t *tree );
int 			del_branch( struct tree_t *tree, struct tree_node_t *parent );
int 			tree_set( struct tree_node_t *parent, enum side_t side, struct tree_node_t *node );

#define default_func( tp, type )				\
int print_##type( FILE *out, void *elem );			\
int cmp_##type( void *elem1, void *elem2 );			\

default_func( d, int );
default_func( lg, double );
default_func( g, float );
default_func( zd, size_t );
default_func( c, char );

int print_string( FILE *out, void *elem );
int cmp_string( void *elem1, void *elem2 );

#undef default_func
#endif /*__TREE_H__*/
