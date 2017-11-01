#include <stdio.h>
#include <stdlib.h>
#include <list_t.h>

struct list_elem
{
	elem_t elem;
	int next;
	int prev;
};

struct list_t
{
	struct list_elem *data;
	int head;
	int tail;
	int free;
	size_t size;
	size_t capacity;
};

int list_configure( struct list_t *list )
{
	int tmp = list -> free;
	for( int i = list -> capacity; i < list -> size - 1; i++ )
	{
		( list -> data )[ tmp ].prev = -1;
		tmp = ( list -> data )[ tmp ].next;
	}
	( list -> data )[ tmp ].next = -1;
	( list -> data )[ tmp ].prev = -1;
	if( list -> capacity != 0 )
	{
		( list -> data )[ list -> head ].prev = -1;
		( list -> data )[ list -> tail ].next = list -> free;
	}
}

int list_construct( struct list_t *list )
{
	list -> data = ( struct list_elem * )calloc( 1, sizeof( struct list_elem ));
	list -> head = 0;
	list -> tail = 0;
	list -> free = 0;
	list -> size = 1;
	list -> capacity = 0;
	( list -> data )[ 0 ].prev = -1;
	( list -> data )[ 0 ].next = -1;
}

int list_resize( struct list_t *list, size_t size )
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

int push_tail( struct list_t *list, elem_t elem )
{
	if( list -> capacity >= list -> size )
	{
		list_resize( list, 2 * list -> size );
	}
	( list -> data )[ list -> free ].elem = elem;
	if( list -> capacity != 0 )
	{
		( list -> data )[ list -> free ].prev = list -> tail;
	}
	list -> tail = list -> free;
	list -> free = ( list -> data )[ list -> free ].next;
	if( list -> capacity == list -> size )
		list -> free = -1;
	( list -> data )[ list -> tail ].next = list -> free;
	list_configure( list );
	list -> capacity ++;
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
}

elem_t list_get_elem( struct list_t *list, int pos )
{
	if( list -> capacity <= 0 )
	{
		return -1;
	}
	elem_t result = ( list -> data )[ pos ].elem;
	( list -> data )[ pos ].elem = -1;
	( list -> data )[ ( list -> data )[ pos ].prev ].next = ( list -> data )[ pos ].next;
	( list -> data )[ ( list -> data )[ pos ].next ].prev = ( list -> data )[ pos ].prev;
	( list -> data )[ pos ].prev = -1;
	( list -> data )[ pos ].next = list -> free;
	( list -> data )[ list -> free ].prev = pos;
	list -> free = pos;
	list -> capacity --;
	return result;
}

int dumper( struct list_t *list )
{
	FILE *dump = fopen( "dump", "w" );
	fprintf( dump, 	"digraph dump\n"
			"{\n"
			"rankdir = LR\n");
	int tmp = list -> head;
	for( int i = 0; i < list -> capacity; i++ )
	{
		fprintf( dump, "Node%d [ shape = record, label = \"<index> index = %d | { <prev> prev = %d | elem = %d | <next> next = %d }\" ]\n", tmp, tmp, ( list -> data )[ tmp ].prev, ( list -> data )[ tmp ].elem, ( list -> data )[ tmp ].next );
		tmp = ( list -> data )[ tmp ].next;
	}

	fprintf( dump, "Node%d [shape = record, label = \"<index> index = %d( free ) | { <prev> prev = %d | FREE | <next> next = %d }\" ]\n", tmp, tmp, ( list -> data )[ tmp ].prev, ( list -> data )[ tmp ].next );
	tmp = ( list -> data )[ tmp ].next;

	for( int i = list -> capacity + 1; i < list -> size; i++ )
	{
		fprintf( dump, "Node%d [ shape = record, label = \"<index> index = %d | { <prev> prev = %d | FREE | <next> next = %d }\" ]\n", tmp, tmp, ( list -> data )[ tmp ].prev, ( list -> data )[ tmp ].next );
		tmp = ( list -> data )[ tmp ].next;
	}

	for( int i = 0; i < list -> size; i++ )
	{
		if( ( list -> data )[ i ].next != -1 )
		{
			fprintf( dump, "Node%d -> Node%d [color = green]\n", i, ( list -> data )[ i ].next );
		}
		if( ( list -> data )[ i ].prev != -1 )
		{
			fprintf( dump, "Node%d -> Node%d [color = blue]\n", i, ( list -> data )[ i ].prev );
		}
	}
	fprintf( dump, "}" );
}
