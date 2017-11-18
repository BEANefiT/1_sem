typedef struct tree_node_t tree_node_t;
typedef struct tree_t tree_t;
typedef int ( *print_function )( void *elem );
typedef int ( *cmp_function )( void *elem1, void *elem2 );

#define tree_create( type, name, func )							\
struct tree_t *name = tree_construct( sizeof( type ), print_##func, cmp_##func );	\
if( name == NULL )									\
	print_log( "ERROR: CANT CREATE TREE\n" );

enum side_t
{
	left,
	right
};
struct tree_t *tree_construct( int sz, print_function func_f, cmp_function cmp_f );
struct tree_node_t *tree_add( struct tree_t *tree, struct tree_node_t *parent, enum side_t side, void *elem );
int change_elem( struct tree_t *tree, struct tree_node_t *node, void *arg );
int del_branch( struct tree_t *tree, struct tree_node_t *parent );
struct tree_node_t *tree_find( struct tree_t *tree, struct tree_node_t *root, void *target );
int dumper( struct tree_t *tree );
void *tree_get_elem( struct tree_node_t *node );
struct tree_node_t *tree_get_next( struct tree_node_t *node, enum side_t side );
struct tree_node_t *tree_get_parent( struct tree_node_t *node );
struct tree_node_t *tree_get_root( struct tree_t *tree );
int tree_print( struct tree_t *tree, void *elem );

#define default_func( tp, type )			\
int print_##type( void *elem )				\
{							\
	printf( "%"#tp, *( type * )elem );		\
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

int print_string( void *elem )
{
	printf( "%s", *( char ** )elem );
}
int cmp_string( void *elem1, void *elem2 )
{
	return strcmp( *( char ** )elem1, *( char ** )elem2 );
}

#undef default_func
