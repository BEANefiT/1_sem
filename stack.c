#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

enum fatal_error_t
{
	NOERR,
	DESTRUCT_STACK,
	RESIZE_STACK,
	FULL_STACK
};

struct stack_t
{
	char name[20];
	size_t elem_size_;
	void *data_;
	size_t size_;
	size_t capacity_;
	enum fatal_error_t error_;
};
static const int POISON = -787787;
static const int capacity_default = 64;

#include "stack.h"

enum error_t check_stack( stack_t *name )
{
	if( (name -> error_) == DESTRUCT_STACK )
	{
		print_log( "FATAL ERROR: STACK DOES NOT EXIST\n" );
		error = FATAL;
		return FATAL;
	}
	if( (name -> error_) == RESIZE_STACK )
	{
		print_log( "WARNING: WORKING WITH MAX_CAPACITY STACK\n" );
		if( (name -> size_) == (name -> capacity_) )
		{
			(name -> error_) = FULL_STACK;
			error = FATAL;
		}
		return noerror;
	}
	if( (name -> error_) == FULL_STACK )
		print_log( "WARNING: WORKING WITH FULL STACK\n" );
	
	return noerror;
}

stack_t *stack_construct( size_t elem_s )
{
	stack_t *name = ( stack_t * )calloc( 1, sizeof( stack_t ) );
	if( name == NULL )
		return name;
	if( name != NULL )
		(name -> error_) = NOERR;
	(name -> elem_size_) = elem_s;
	(name -> size_) = 0;
	(name -> capacity_) = capacity_default;
	(name -> data_) = ( void * )calloc( (name -> capacity_), (name -> elem_size_) );
	return name;
}

enum error_t 	stack_destruct( stack_t *name )
{
	if( name == NULL )
	{
		print_log( "ERROR: stack_t* = %p\n", name );
		error = BAD_ARG;
		return BAD_ARG;
	}
	if( check_stack( name ) == FATAL )
		return FATAL;
	
	free( name -> data_ );
	(name -> size_) = POISON;
	(name -> capacity_) = POISON;
	(name -> error_) = DESTRUCT_STACK;
	return noerror;
}

enum error_t push( stack_t* name, const void* value )
{

	if( name == NULL )
	{
		print_log( "ERROR: stack_t* = %p\n", name );
		error = BAD_ARG;
		return BAD_ARG;
	}
	if( check_stack( name ) == FATAL )
		return FATAL;

	if( value == NULL )
	{
		print_log( "ERROR: const void* = %p\n", value );
		error = BAD_ARG;
		return BAD_ARG;
	}
	if( (name -> size_) > (name -> capacity_) )
		Do( resize( name, (name -> capacity_) * 2 ) );
	memcpy( ( (name -> data_)  +  ( (name -> size_)++ ) * ( name -> elem_size_ )), value, name -> elem_size_ );
	return noerror;
}

void* top( stack_t* name )
{
	if( name == NULL )
	{
		print_log( "ERROR: stack_t* = %p\n", name );
		error = BAD_ARG;
		return BAD_ARG;
	}
	if( check_stack( name ) == FATAL )
		return NULL;

	if( (name -> size_) == 0 )
	{
		
		print_log( "ERROR: TRYING TO TOP FROM EMPTY STACK\n" );
		error = EMPTY_STACK;
		return NULL;
	}
	void* dest = ( void* ) calloc ( 1, name -> elem_size_ );
	if( dest == NULL )
	{
		print_log( "ERROR: UNEXPECTED ERROR\n" );
		error = UNEXPECTED;
		return NULL;
	}
	memcpy( dest, ( name -> data_ ) + ( (name -> size_) - 1 ) * ( name -> elem_size_ ), name -> elem_size_ );
	return dest;
}

void* pop( stack_t* name )
{
	if( name == NULL )
	{
		print_log( "ERROR: stack_t* = %p\n", name );
		error = BAD_ARG;
		return BAD_ARG;
	}
	if( check_stack( name ) == FATAL )
		return NULL;

	if( (name -> size_) == 0 )
	{
		
		print_log( "ERROR: TRYING TO POP FROM EMPTY STACK\n" );
		error = EMPTY_STACK;
		return NULL;
	}

	void* dest = ( void* ) calloc ( 1, name -> elem_size_);
	if( dest == NULL )
	{
		print_log( "ERROR: UNEXPECTED ERROR\n" );
		error = UNEXPECTED;
		return NULL;
	}
	memcpy( dest, ( name -> data_ ) + ( --(name -> size_) ) * ( name -> elem_size_ ), name -> elem_size_ );
	return dest;
}
enum error_t resize( stack_t* name, size_t capacity_new )
{
	if( name == NULL )
	{
		print_log( "ERROR: stack_t* = %p\n", name );
		error = BAD_ARG;
		return BAD_ARG;
	}

	if( check_stack( name ) == FATAL )
	{
		print_log( "FATAL ERROR: CANNOT RESIZE STACK\n" );
		return FATAL;
	}
	void* data_tmp = ( void* ) calloc ( capacity_new, name -> elem_size_ );
	if( data_tmp == NULL)
	{
		if( capacity_new == (name -> capacity_) )
		{
			(name -> error_) = RESIZE_STACK;
			print_log( "ERROR: CANNOT RESIZE\n");
			return FATAL;
		}
	print_log( "ERROR: CANNOT RESIZE TO capacity_new = %zd\n", capacity_new );
	Do( resize( name, capacity_new - 1 ) );
	}	
	if( data_tmp != NULL )
	{
		free( name -> data_ );
		(name -> data_) = data_tmp;
		(name -> capacity_) = capacity_new;
		return noerror;
	}
}
size_t size( stack_t* name )
{
	if( name == NULL )
	{
		print_log( "ERROR: stack_t* = %p\n", name );
		return POISON;
	}
	return name -> size_;
}
size_t 	capacity( stack_t* name )
{
	if( name == NULL )
	{
		print_log( "ERROR: stack_t* = %p\n", name );
		return POISON;
	}
	return name -> capacity_;
}
int	empty( stack_t* name )
{
	if( name == NULL )
	{
		print_log( "ERROR: stack_t* = %p\n", name );
		return POISON;
	}
	return ( name -> size_ ) > 0;
}
