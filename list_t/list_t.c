#include <stdio.h>

typedef int elem_t;

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

int list_construct( struct list_t *list )
{
	list -> data = ( struct list_elem * )calloc( 1, sizeof( struct list_elem ));
	list -> head = 0;
	list -> tail = 0;
	list -> free = 0;
	list -> size = 1;
	list -> capacity = 0;
	(list -> data)[ 0 ].prev = 0;
	for( int i = 0; i < sz; i++ )
	{
		( list -> data )[ i ].next = i + 1;
		( list -> data )[ i ].elem = -1;
	}
}

int list_resize( struct list_t *list, size_t size )
{
	list -> data = ( struct list_elem * )calloc( size, sizeof( struct list_elem ) );
	list -> size = size;
}

int push_tail( struct list_t *list, int elem )
{
	if( list -> capacity >= list -> size )
	{
		list_resize( list, 2 * list -> size );
	}
	( list -> data )[ free ].elem = elem;
	( list -> data )[ free ].prev = tail;
	( list -> data )[ head ].prev = ( list -> data )[ free ].next;
	tail = free;
	free = ( list -> data )[ free ].next;
	list -> capacity ++;
}

int push_head( struct list_t *list, int elem )
{
	if( list -> capacity >= list -> size )
	{
		list_resize( list, 2 * list -> size );
	}
	( list -> data )[ free ].elem = elem;
	( list -> data )[ free ].next = head;
	head = free;
	free = ( list -> data )[ free ].next;
	( list -> data )[ tail ].next = free;
	( list -> data )[ head ].prev = free;
	list -> capacity ++;
}

int pop_tail( struct list_t *list, int *dest )
{
	if( list -> capacity <= 0 )
	{
		return tail;
	}
	*dest = ( list -> data )[ tail ].elem;
	( list -> data )[ tail ].elem = -1;
	free = tail;
	tail = ( list -> data )[ tail ].prev;
	( list -> data )[ head ].prev = free;
	list -> capacity --;
	return tail;
}

int pop_head( struct list_t *list, int *dest )
{
	if( list -> capacity <= 0 )
	{
		return tail;
	}

	*dest = ( list -> data )[ head ].elem;
	( list -> data )[ head ].elem = -1;
	free = head;
	( list -> data )[ free ].prev = -1;
	( list -> data )[ free ].next = ( list -> data )[ tail ].next;
	head = ( list -> data )[ head ].next;
	( list -> data )[ tail ].next = free;
	list -> capacity --;
	return head;
}

int insert( struct list_t *list, int pos, int elem )
{
	( list -> data )[ free ].elem = elem;
	( list -> data )[ free ].prev = pos;
	( list -> data )[ free ].next = ( list -> data )[ pos ].next;
	( list -> data )[ ( list -> data )[ pos ].next ].prev = free;
	( list -> data )[ pos ].next = free;
	free = ( list -> data )[ free ].next;
	( list -> data )[ head ].prev = free;
	( list -> data )[ tail ].next = free;
}

int *sort( struct list_t *list )
{
	int *a = ( int * )calloc( sz, sizeof( int ) );
	a[ 0 ] = head;
	a[ sz - 1 ] = tail;
	for( int i = 1; i < sz - 1; i++ )
	{
		a[ i ] = ( list -> data )[ a[ i - 1 ] ].next;
	}
	return a;
}

