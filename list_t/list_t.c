#include <stdio.h>

struct list_elem
{
	int elem;
	int next;
	int prev;
};

struct list_t
{
	struct list_elem *data;
	int head;
	int tail;
	int free;
};

int list_construct( struct list_t *list, size_t sz )
{
	list -> data = ( struct list_elem * )calloc( sz, sizeof( struct list_elem ));
	list -> head = 0;
	list -> tail = 0;
	list -> free = 0;
	(list -> data)[ 0 ].prev = 0;
	for( int i = 0; i < sz; i++ )
	{
		( list -> data )[ i ].next = i + 1;
		( list -> data )[ i ].elem = -1;
	}
}

int push_tail( struct list_t *list, int elem )
{
	{
		( list -> data )[ free ].elem = elem;
		( list -> data )[ free ].prev = tail;
		( list -> data )[ head ].prev = ( list -> data )[ free ].next;
		tail = free;
		free = ( list -> data )[ free ].next;
	}
}

int push_head( struct list_t *list, int elem )
{
	( list -> data )[ free ].elem = elem;
	( list -> data )[ free ].next = head;
	( list -> data )[ tail ].next = ( list -> data )[ free ].next;
	head = free;
	free = ( list -> data )[ free ].next;
}

int pop_tail( struct list_t *list, int *dest )
{
	*dest = ( list -> data )[ tail ].elem;
	( list -> data )[ tail ].elem = -1;
	free = tail;
	tail = ( list -> data )[ tail ].prev;
	return tail;
}

int pop_head( struct list_t *list, int *dest )
{
	*dest = ( list -> data )[ head ].elem;
	( list -> data )[ head ].elem = -1;
	free = head;
	head = ( list -> data )[ head ].next;
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

