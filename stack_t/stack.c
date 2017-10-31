#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../log/log.h"

enum fatal_error_t
{
	NOERR,
	DESTRUCT_STACK,
	RESIZE_STACK,
	FULL_STACK
};

const int POISON = -787787;
const int capacity_default = 64;
const int CANRY = 123654;

#define Stack_Name_size 20
struct stack_t
{
	char 	name[Stack_Name_size];
	size_t 	elem_size_;
	void*	data_;
	size_t 	size_;
	size_t 	capacity_;
	enum 	fatal_error_t error_;
	int*	canry1_;
	int*	canry2_;
	int*	data_canry1_;
	int* 	data_canry2_;
	size_t	hash_;
};
#undef Stack_Name_size

enum error_t	check_stack( struct stack_t *name );
size_t		hash( struct stack_t *name );
size_t		data_hash( struct stack_t *name );

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
	
	if( *( name -> canry1_ ) != CANRY )
		print_log( "WARNING: LEFT CANRY IS BAD\n" );
	
	if( *( name -> canry2_ ) != CANRY )
		print_log( "WARNING: RIGHT CANRY IS BAD\n" );
	
	if( *( name -> data_canry1_ ) != CANRY )
		print_log( "WARNING: LEFT DATA-CANRY IS BAD\n" );

	if( *( name -> data_canry2_ ) != CANRY )
		print_log( "WARNING: RIGHT DATA-CANRY IS BAD\n" );
	
	if( name -> hash_ != hash( name ) )
		print_log( "WARNING: BAD HASH\n" );

	return noerror;
}

stack_t *stack_construct( size_t elem_s )
{
	stack_t *name = ( stack_t * )( calloc( 1, sizeof( stack_t ) + sizeof( int ) * 2 ) + sizeof( int ) );
	name -> canry1_ = ( int* )( ( void* )name - sizeof( int ) );
	name -> canry2_ = ( int* )( ( void* )name + sizeof( stack_t ) );
	*( name -> canry1_ ) = CANRY;
	*( name -> canry2_ ) = CANRY;
	if( name == NULL )
		return name;
	if( name != NULL )
		(name -> error_) = NOERR;
	(name -> elem_size_) = elem_s;
	(name -> size_) = 0;
	(name -> capacity_) = capacity_default;
	(name -> data_) = calloc( 1, (name -> capacity_) *  (name -> elem_size_) + sizeof( int ) * 2 ) + sizeof( int );
	(name -> data_canry1_) = ( int* )( name -> data_ - sizeof( int ) );
	(name -> data_canry2_) = ( int* )( name -> data_ + (name -> capacity_) * (name -> elem_size_) );
	*( name -> data_canry1_ ) = CANRY;
	*( name -> data_canry2_ ) = CANRY;
	(name -> hash_) = hash( name );
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
	(name -> data_canry1_) = NULL;
	(name -> data_canry2_) = NULL;
	(name -> canry1_) = NULL;
	(name -> canry2_) = NULL;
	(name -> hash_) = POISON;
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
	name -> hash_ = hash( name );

	if( check_stack( name ) == FATAL )
		return FATAL;
	
	return noerror;
}

enum error_t top( stack_t* name, void* dest )
{
	if( name == NULL )
	{
		print_log( "ERROR: stack_t* = %p\n", name );
		error = BAD_ARG;
		return BAD_ARG;
	}
	if( check_stack( name ) == FATAL )
		return FATAL;

	if( (name -> size_) == 0 )
	{
		
		print_log( "ERROR: TRYING TO TOP FROM EMPTY STACK\n" );
		error = EMPTY_STACK;
		return FATAL;
	}
	if( dest == NULL )
	{
		print_log( "ERROR: UNEXPECTED ERROR\n" );
		error = UNEXPECTED;
		return FATAL;
	}
	memcpy( dest, ( name -> data_ ) + ( (name -> size_) - 1 ) * ( name -> elem_size_ ), name -> elem_size_ );
	name -> hash_ = hash( name );

	if( check_stack( name ) == FATAL )
		return FATAL;

	return noerror;
}

enum error_t pop( stack_t* name, void* dest )
{
	if( name == NULL )
	{
		print_log( "ERROR: stack_t* = %p\n", name );
		error = BAD_ARG;
		return BAD_ARG;
	}
	if( check_stack( name ) == FATAL )
		return FATAL;

	if( (name -> size_) == 0 )
	{
		
		print_log( "ERROR: TRYING TO POP FROM EMPTY STACK\n" );
		error = EMPTY_STACK;
		return EMPTY_STACK;
	}

	if( dest == NULL )
	{
		print_log( "ERROR: UNEXPECTED ERROR\n" );
		error = UNEXPECTED;
		return UNEXPECTED;
	}
	memcpy( dest, ( name -> data_ ) + ( --(name -> size_) ) * ( name -> elem_size_ ), name -> elem_size_ );
	name -> hash_ = hash( name );

	if( check_stack( name ) == FATAL )
		return FATAL;

	return noerror;
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
	void* data_tmp = calloc ( 1, capacity_new * name -> elem_size_ + sizeof( int ) * 2 );
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
		(name -> data_canry1_) = ( int* )( name -> data_ - sizeof( int ) );
		(name -> data_canry2_) = ( int* )( name -> data_ + name -> capacity_ * name -> elem_size_ );
		*(name -> data_canry1_) = CANRY;
		*(name -> data_canry2_) = CANRY;
		(name -> hash_) = hash( name );
		if( check_stack( name ) == FATAL )
			return FATAL;

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

#ifdef __DEBUG__
size_t	hash( stack_t* name )
{
	return( 1 * ( int )name -> data_ + 2 * name -> size_ + 3 * name -> capacity_ + 4 * data_hash( name ) );
}

size_t	data_hash( stack_t* name )
{
	size_t data_hash_ = 0;
	for( size_t i = 0; i < (name -> capacity_); i++ )
		data_hash_ += *( ( int* )( name -> data_ + name -> elem_size_ * i ) );
		data_hash_ *= 2;
	return data_hash_;
}
#else
size_t hash( stack_t *name )
{
	return 0;
}
size_t data_hash( stack_t *name )
{
	return 0;
}
#endif /*__DEBUG__*/
