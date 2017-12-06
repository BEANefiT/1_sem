#include <stdio.h>
#include <stdlib.h>
#include "./../log/log.h"
#include "list_t.h"

struct list_node_t
{
	void*	elem;
	int 	next;
	int 	prev;
};

struct list_t
{
	print_function 		printer;
	struct list_node_t*	data;
	int 			head;
	int 			tail;
	int 			free;
	size_t 			size;
	size_t 			capacity;
};

int list_construct( struct list_t *list, print_function print_f )
{
	check_pointer( list, 1 );

	list -> printer  =    print_f;
	list -> data 	 =    ( struct list_node_t * )calloc( 1, sizeof( struct list_node_t ) );
	list -> head 	 =    0;
	list -> tail 	 =    0;
	list -> free 	 =    0;
	list -> size 	 =    0;
	list -> capacity =    1;

	list -> data[ 0 ].prev = -1;
	list -> data[ 0 ].next = -1;
}

int push_tail( struct list_t *list, void *elem )
{
	if( list -> capacity <= list -> size )
	{
		//list_resize( list, 2 * list -> size );
	}

	list -> data[ list -> free ].elem = elem;

	if( list -> size != 0 )
	{
		list -> data[ list -> free ].prev = list -> tail;
		list -> data[ list -> tail ].next = list -> free;
	}

	list -> tail = list -> free;
	
	if( list -> size != 0 )
		list -> free = list -> data[ list -> free ].next;

	list -> size ++;
}

/*int list_resize( struct list_t *list, size_t size )
{
	list -> data = ( struct list_elem * )realloc( list -> data, size * sizeof( struct list_elem ) );
	int tmp = list -> free;
	for( int i = list -> capacity; i < list -> size - 1; i++ )
	{
		tmp = ( list -> data )[ tmp ].next;
	}
	( list -> data )[ tmp ].next = list -> size;
	for( int i = list -> size; i < size - 1; i++ )
	{
		( list -> data )[ i ].next = i + 1;
	}
	list -> size = size;
	list_configure( list );
}

int push_head( struct list_t *list, elem_t elem )
{
	if( list -> capacity >= list -> size )
	{
		list_resize( list, 2 * list -> size );
	}
	( list -> data )[ list -> free ].elem = elem;
	int tmp = list -> free;
	( list -> data )[ list -> free ].next = list -> head;
	list -> head = list -> free;
	list -> free = ( list -> data )[ tmp ].next;
	( list -> data )[ list -> tail ].next = list -> free;
	( list -> data )[ list -> head ].prev = list -> free;
	list -> capacity ++;
}

int pop_tail( struct list_t *list, elem_t *dest )
{
	if( list -> capacity <= 0 )
	{
		return list -> tail;
	}
	*dest = ( list -> data )[ list -> tail ].elem;
	( list -> data )[ list -> tail ].elem = -1;
	list -> free = list -> tail;
	list -> tail = ( list -> data )[ list -> tail ].prev;
	( list -> data )[ list -> head ].prev = list -> free;
	list -> capacity --;
	return list -> tail;
}

int pop_head( struct list_t *list, elem_t *dest )
{
	if( list -> capacity <= 0 )
	{
		return list -> head;
	}

	*dest = ( list -> data )[ list -> head ].elem;
	( list -> data )[ list -> head ].elem = -1;
	list -> free = list -> head;
	( list -> data )[ list -> free ].prev = -1;
	( list -> data )[ list -> free ].next = ( list -> data )[ list -> tail ].next;
	list -> head = ( list -> data )[ list -> head ].next;
	( list -> data )[ list -> tail ].next = list -> free;
	list -> capacity --;
	return list -> head;
}

int insert( struct list_t *list, int pos, elem_t elem )
{
	if( list -> capacity >= list -> size )
	{
		list_resize( list, 2 * list -> size );
	}
	( list -> data )[ list -> free ].elem = elem;
	( list -> data )[ list -> free ].prev = pos;
	( list -> data )[ list -> free ].next = ( list -> data )[ pos ].next;
	( list -> data )[ ( list -> data )[ pos ].next ].prev = list -> free;
	( list -> data )[ pos ].next = list -> free;
	list -> free = ( list -> data )[ list -> free ].next;
	( list -> data )[ list -> head ].prev = list -> free;
	( list -> data )[ list -> tail ].next = list -> free;
	list -> capacity ++;
}

int compress( struct list_t *list )
{
	int *tmp = ( int * )calloc( list -> size, sizeof( int ) );
	tmp[ 0 ] = list -> head;
	tmp[ list -> size - 1 ] = list -> tail;
	for( int i = 1; i < list -> size - 1; i++ )
	{
		tmp[ i ] = ( list -> data )[ tmp[ i - 1 ] ].next;
	}
	struct list_elem *newdata = ( struct list_elem * )calloc( list -> size, sizeof( struct list_elem ) );
	for( int i = 0; i < list -> size; i++ )
	{
		newdata[ i ].elem = ( list -> data )[ tmp[ i ] ].elem;
		newdata[ i ].next = i + 1;
		newdata[ i ].prev = i - 1;
	}
	list -> free = list -> capacity;
	list -> tail = list -> free - 1;
	list -> head = 0;
	newdata[ list -> size - 1 ].next = -1;
	newdata[ 0 ].prev = list -> free;
	list -> data = newdata;
}*/

void *list_get_elem( struct list_node_t *node )
{
	check_pointer( node, NULL );

	return node -> elem;
}

int list_print( struct list_t *list, FILE *out, void *elem )
{
	check_pointer( list, 1 );
	check_pointer( out, 1 );
	check_pointer( elem, 1 );

	list -> printer( out, elem );
}

#define default_printer( tp, type )			\
int print_##type( FILE *out, void *elem )		\
{							\
	fprintf( out, "%"#tp, *( type * )elem );	\
}

default_printer( d, int );
default_printer( lg, double );
default_printer( c, char );
default_printer( zd, size_t );
default_printer( g, float );

#undef default_printer

int dumper( FILE *dump, struct list_t *list, int index )
{
	fprintf( dump, "node%d [ shape = record, label = \" index = %d | { prev = %d | elem = ", index, index, list -> data[ index ].prev );
	list_print( list, dump, &( list -> data[ index ] ) );
	fprintf( dump, " | next = %d }\" ]\n", list -> data[ index ].next );
	fprintf( dump, "node%d -> node%d\n", index, list -> data[ index ].next );

	if( list -> data[ index ].next != -1 )
		dumper( dump, list, list -> data[ index ].next );
}

int dump_it( struct list_t *list )
{
	FILE *dump = fopen( "dump", "w" );
	fprintf( dump, 	"digraph dump\n"
			"{\n"
			"rankdir = LR\n");

	dumper( dump, list, list -> head );

	fprintf( dump, "}" );
}
