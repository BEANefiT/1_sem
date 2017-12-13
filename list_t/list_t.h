typedef struct list_t 		list_t;
typedef struct list_node_t	list_node_t;

typedef int ( *print_function )( FILE *, void * );

struct list_t* 	list_construct( int elem_size, print_function print_f );
void* 		list_get_elem( struct list_node_t node );
int 		list_resize( struct list_t *list, size_t new_sz );
int 		push_tail( struct list_t *list, void* elem );
int 		push_head( struct list_t *list, void* elem );
int 		pop_tail( struct list_t *list, void *dest );
int 		pop_head( struct list_t *list, void *dest );
//int 		insert( struct list_t *list, int pos, void* elem );
//int 		compress( struct list_t *list );
int 		dump_it( struct list_t *list );

int print_int( FILE *, void * );
int print_double( FILE *, void * );
int print_char( FILE *, void * );
int print_size_t( FILE *, void * );
int print_float( FILE *, void * );
