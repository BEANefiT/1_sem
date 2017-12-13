#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	int			elem_sz;
	size_t 			size;
	size_t 			capacity;
};

struct list_t *list_construct( int elem_size, print_function print_f )
{
	struct list_t *list = ( struct list_t * )calloc( 1, sizeof( struct list_t ) );
	check_pointer( list, NULL );

	list -> printer  =    print_f;
	list -> data 	 =    ( struct list_node_t * )calloc( 1, sizeof( struct list_node_t ) );
	list -> head 	 =    0;
	list -> tail 	 =    0;
	list -> free 	 =    0;
	list -> elem_sz  =    elem_size;
	list -> size 	 =    0;
	list -> capacity =    1;

	list -> data[ 0 ].prev = -1;
	list -> data[ 0 ].next = -1;

	return list;
}

int push_tail( struct list_t *list, void *elem )
{
	check_pointer( list, 1 );
	check_pointer( elem, 1 );

	if( list -> capacity <= list -> size )
	{
		list_resize( list, 2 * list -> capacity );
	}

	list -> data[ list -> free ].elem = calloc( 1, list -> elem_sz );
	check_pointer( list -> data[ list -> free ].elem, 1 );

	memcpy( list -> data[ list -> free].elem, elem, list -> elem_sz );

	if( list -> size != 0 )
	{
		list -> data[ list -> free ].prev = list -> tail;
		list -> data[ list -> tail ].next = list -> free;
	}

	list -> tail = list -> free;
	
	list -> free = list -> data[ list -> free ].next;
	list -> data[ list -> tail ].next = list -> free;
	list -> data[ list -> head ].prev = list -> free;

	list -> size ++;

	return 0;
}

int list_resize( struct list_t *list, size_t new_sz )
{
	check_pointer( list, 1 );

	list -> data = ( struct list_node_t * )realloc( list -> data, new_sz * sizeof( struct list_node_t ) );
	check_pointer( list -> data, 1 );

	int tmp = list -> free;

	for( int i = list -> size; i < list -> capacity - 1; i++ )
	{
		tmp = list -> data[ tmp ].next;
	}

	if( tmp == -1 )
	{
		list -> free = list -> size;

		tmp = list -> free;
	}

	list -> data[ tmp ].next = list -> size + 1;

	for( int i = list -> size + 1; i < new_sz - 1; i ++ )
		list -> data[ i ].next = i + 1;

	list -> data[ new_sz - 1 ].next = -1;

	list -> data[ list -> head ].prev = list -> free;
	list -> data[ list -> tail ].next = list -> free;
	list -> capacity = new_sz;
}

int push_head( struct list_t *list, void *elem )
{
	check_pointer( list, 1 );
	check_pointer( elem, 1 );

	if( list -> capacity <= list -> size )
	{
		list_resize( list, 2 * list -> capacity );
	}

	list -> data[ list -> free ].elem = calloc( 1, list -> elem_sz );
	check_pointer( list -> data[ list -> free ].elem, 1 );

	memcpy( list -> data[ list -> free].elem, elem, list -> elem_sz );

	int newfree = list -> data[ list -> free ].next;

	if( list -> size != 0 )
	{
		list -> data[ list -> free ].next = list -> head;
		list -> data[ list -> head ].prev = list -> free;
	}

	list -> head = list -> free;
	
	list -> free = newfree;
	list -> data[ list -> tail ].next = list -> free;
	list -> data[ list -> head ].prev = list -> free;

	list -> size ++;

	return 0;
}

int pop_tail( struct list_t *list, void *dest )
{
	check_pointer( list, 1 );
	check_pointer( dest, 1 );

	if( list -> size <= 0 )
	{
		return -1;
	}

	memcpy( dest, list -> data[ list -> tail ].elem, list -> elem_sz );

	free( list -> data[ list -> tail ].elem );
	list -> data[ list -> tail ].elem = NULL;

	if( list -> size > 0 )
	{
		list -> data[ list -> head ].prev = list -> tail;
		list -> free = list -> tail;
		list -> tail = list -> data[ list -> tail ].prev;
	}
	
	list -> data[ list -> free ].prev = -1;

	list -> size--;
	return 0;
}

int pop_head( struct list_t *list, void *dest )
{
	check_pointer( list, 1 );
	check_pointer( dest, 1 );

	if( list -> size <= 0 )
	{
		return -1;
	}

	memcpy( dest, list -> data[ list -> head ].elem, list -> elem_sz );

	free( list -> data[ list -> head ].elem );
	list -> data[ list -> head ].elem = NULL;

	int oldfree = list -> free;

	if( list -> size > 0 )
	{
		list -> data[ list -> tail ].next = list -> head;
		list -> free = list -> head;
		list -> head = list -> data[ list -> head ].next;
	}

	list -> data[ list -> free ].prev = -1;
	list -> data[ list -> free ].next = oldfree;

	list -> size--;
	return 0;
}

int insert( struct list_t *list, int pos, void *elem )
{
	check_pointer( list, 1 );
	check_pointer( elem, 1 );

	if( list -> size >= list -> capacity )
	{
		list_resize( list, 2 * list -> capacity );
	}

	if( pos == list -> tail )
		return push_tail( list, elem );

	if( list -> data[ pos ].elem == NULL )
		return 1;

	list -> data[ list -> free ].elem = calloc( 1, list -> elem_sz );
	check_pointer( list -> data[ list -> free ].elem, 1 );

	memcpy( list -> data[ list -> free].elem, elem, list -> elem_sz );

	int next = list -> data[ pos ].next;
	int nextfree = list -> data[ list -> free ].next;

	list -> data[ list -> free ].prev = pos;
	list -> data[ list -> free ].next = next;

	list -> data[ pos ].next = list -> free;
	list -> data[ next ].prev = list -> free;

	list -> free = nextfree;
	list -> data[ list -> head ].prev = list -> free;
	list -> data[ list -> tail ].next = list -> free;

	list -> size++;
	
	return 0;
}
/*
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

void *list_get_elem( struct list_node_t node )
{
	return node.elem;
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
	fprintf( dump, "node%d [ shape = record, label = \" index = %d | { prev = %d | ", index, index, list -> data[ index ].prev );

	if( list -> data[ index ].elem == NULL )
		fprintf( dump, "FREE" );

	else
	{
		fprintf( dump, "elem = " );
		list_print( list, dump, list_get_elem( list -> data[ index ] ) );
	}

	if( index == list -> head )
		fprintf( dump, "( head )" );

	if( index == list -> tail )
		fprintf( dump, "( tail )" );

	fprintf( dump, " | next = %d }\" ]\n", list -> data[ index ].next );

	if( list -> data[ index ].next != -1 )
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
