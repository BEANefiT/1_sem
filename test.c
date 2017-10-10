#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "stack.h"
int main()
{
	stack( INT, s1 );
	for( int i = 1; i <= 4; i++ )
	{
		Do( push( s1, &i ) );
		
	}
	int* dest = 0;
	for( int i = 0; i < 4; i++ )
	{
		Do( dest = ( int* )top( s1 ) );
		printf( "%d ", *dest );
		Do( dest = ( int* )pop( s1 ) );
		printf( "%d ", *dest );
	}
	printf( "\n" );
	fclose( log );
	return 0;
}
