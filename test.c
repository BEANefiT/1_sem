#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "log.h"

int main()
{
	log_constr();
	stack( int, s1 );
	int* dest = 0;
	Do( dest = ( int* )top(s1) );
	for( int i = 1; i <= 4; i++ )
	{
		Do( push( s1, &i ) );
		
	}
	for( int i = 0; i < 4; i++ )
	{
		Do( dest = ( int* )top( s1 ) );
		printf( "%d ", *dest );
		Do( dest = ( int* )pop( s1 ) );
		printf( "%d ", *dest );
	}
	printf( "\n" );
	log_destr();
	return 0;
}
