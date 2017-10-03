#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "stack.h"

#define ASSERT_OK() \
	do{ \
		if ( !size_ ) \
			error_ = error_emptystack; \
		if ( root_ == nullptr ) \
			error_ = error_alloc; \
		fprintf ( logs, "%s\t%s() ", __FILE__, __func__ ); \
		if ( mode == start ) \
			fprintf ( logs, "|start|:\t" ); \
		else \
			fprintf ( logs, "|end|:\t" ); \
		if ( error_ != noerror ){ \
			fprintf ( logs, "ERROR: " ); \
			switch ( error_ ){ \
						 \
				case error_alloc: \
					fprintf ( logs, "BAD ALLOCATION\n" ); \
					break; \
				case  error_emptystack: \
					fprintf ( logs, "TRYING TO OUTPUT NON-EXISTING ELEMENT\n" ); \
					break; \
			} \
			fprintf ( logs, "\t\t\t\troot_pointer = %0xd, capasity = %d, size = %d\n\n", root_, capasity_, size_ ); \
			error_ = noerror; \
		} \
		else \
			fprintf ( logs, "OK\n" ); \
	} while (0)
//#endif /* DEBUG */

const int		start = 0;	//for "log.txt"
const int		end = 1;

const int		poison = INT_MIN;
const int		capasity_default = 64;
stack_t :: value_type*	root_ = {};
int 			size_ = poison;
int 			capasity_ = poison;
int 			error_ = poison;
bool			mode = start;
FILE*			logs = fopen ( "log.txt", "a" );

stack_t :: stack_t (){

	capasity_ = capasity_default;
	root_ = ( value_type* ) calloc ( capasity_, sizeof ( stack_t :: value_type ) );
	size_ = 0;
	error_ = noerror;

}

stack_t :: ~stack_t (){

	assert ( root_ != nullptr );
	free ( root_ );
	capasity_ = poison;
	size_ = poison;
	error_ = poison;
	fclose ( logs );

}

int stack_t :: resize ( int capasity_new ){

	mode = start;
	ASSERT_OK();
	assert ( capasity_new >= 0 );
	assert ( root_ != nullptr );
	if ( capasity_new < size_ ){
		error_ = error_loss;
		fprintf ( logs, "%s\t%s\tERROR:\tDATA LOSSES\n", __FILE__, __func__ );
		fprintf ( logs, "\t\t\t\t capasity = %d, size = %d, new_capasity = %d\n\n", capasity_, size_, capasity_new );
		size_ = capasity_new;
	}
	value_type* root_tmp = ( value_type* ) calloc ( capasity_new, sizeof ( value_type ) );
	if ( root_tmp != nullptr ){
		free ( root_tmp );
		capasity_ = capasity_new;
		root_ = (value_type* ) realloc ( root_, capasity_ * sizeof ( value_type ) );
	}
	else{
		error_ = error_alloc;
		fprintf ( logs, "%s\t%s\tERROR:\tBAD ALLOCATION\n", __FILE__, __func__ );
		fprintf ( logs, "\t\t\t\tcapasity_new = %d", capasity_new );
		error_ = noerror;
	}
	mode = end;
	ASSERT_OK();
	return true;

}

bool stack_t :: push ( value_type value ){

	//size_ += 1;
	mode = start;
	ASSERT_OK();
	//size_ -= 1;
	if ( ( size_ + 1 ) >= capasity_ )
		( this -> resize ) ( capasity_ * 2 );
	root_ [ size_ ++ ] = value;
	mode = end;
	ASSERT_OK();
	return true;
			
}

stack_t :: value_type stack_t :: top (){

	mode = start;
	ASSERT_OK();
	return ( root_ [ size_ - 1 ] );

}

stack_t :: value_type stack_t :: pop (){

	mode = start;
	ASSERT_OK();
	if ( size_ == 0 )
		return INT_MIN;
	if ( size_ - 1 < capasity_ / 2 && capasity_ > capasity_default )
		( this -> resize ) ( capasity_ / 2 );
	size_ -= 1;
	mode = end;
	ASSERT_OK();
	return ( root_ [ size_ ] );

}

int stack_t :: size (){

	return ( size_ );

}

bool stack_t :: empty (){

	return ( size_ > 0 );

}

int stack_t :: capasity (){

	return ( capasity_ );

}

bool stack_t :: clear (){

	size_ = poison;

	return true;

}
