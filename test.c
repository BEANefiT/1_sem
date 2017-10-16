#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "log.h"

int main()
{
	log_constr();
	stack( int, s1 );
	int* dest = ( int* )calloc( 1, sizeof( int ) );
	Do( top(s1, dest) );
	for( int i = 1; i <= 4; i++ )
	{
		Do( push( s1, &i ) );
		
	}
	for( int i = 0; i < 4; i++ )
	{
		Do( top( s1, dest ) );
		printf( "%d ", *dest );
		Do( pop( s1, dest ) );
		printf( "%d ", *dest );
	}
	printf( "\n" );
	log_destr();
	return 0;
}
