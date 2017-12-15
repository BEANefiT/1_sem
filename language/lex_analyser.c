#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex_analyser.h"
#include "./../log/log.h"

#define MAX_LEXEMS_COUNT 8192

int make_lexems( struct analyser_t *analyser )
{
	check_pointer( analyser, 1 );

	char *src = analyser -> src;
	check_pointer( src, 1 );

	struct lex_t **lexems = ( struct lex_t ** )calloc( MAX_LEXEMS_COUNT, sizeof( struct lex_t * ) );
	check_pointer( lexems, 1 );
	
	while( src - analyser -> src <= analyser -> src_sz )
	{
		char elem = *src;
		
		if( isTRASH )
		{
			src++;

			continue;
		}

		check_for_lexem( VAL,  1 )

		check_for_lexem( OPER, 2 )

		check_for_lexem( BR,   5 )
		
		char *word = ( char * )calloc( LEX_ANALYSER_MAX_VALUE_LENGTH, sizeof( char ) );
		check_pointer( word, 1 );

		int word_sz = 0;

		while( isLETTER || isVAL )
		{
			word[ word_sz++ ] = *(src++);
		}

		if( !word_sz )
		{
			printf( "Syntax error: unrecognized symbol '%c'\n", elem );

			return 1;
		}

		word = ( char * )realloc( word, word_sz * sizeof( char ) );

		check_for_word( KW,   3 )
		
		check_for_word( FUNC, 6 )

		check_for_word( VAR,  4 )
	}

	return 0;
}

char *getVAL( char *src, struct lex_t *lexem )
{
	check_pointer( lexem, NULL );
	check_pointer( src, NULL );

	int tmp = 0;

	while( *src >= '0' && *src <= '9' )
		lexem -> value[ tmp ] = *( src++ );

	return src;
}

char *getOPER( char *src, struct lex_t *lexem )
{
	check_pointer( lexem, NULL );
	check_pointer( src, NULL );

	*lexem -> value = *(src++);

	return src;
}

char *getBR( char *src, struct lex_t *lexem )
{
	check_pointer( lexem, NULL );
	check_pointer( src, NULL );

	*lexem -> value = *( src++ );

	return src;
}

int getKW( struct lex_t *lexem, char *word, int word_sz )
{
	check_pointer( lexem, 1 );
	check_pointer( word, 1 );

	if( !strcmp( word, "esli" ) )
		*lexem -> value = '1';

	if( !strcmp( word, "inache" ) )
		*lexem -> value = '2';

	if( !strcmp( word, "poka" ) )
		*lexem -> value = '3';

	return 0;
}

int getFUNC( struct lex_t *lexem, char *word, int word_sz )
{
	check_pointer( lexem, 1 );
	check_pointer( word, 1 );

	for( int i = 0; i < word_sz; i++ )
		lexem -> value[ i ] = word[ i ];

	return 0;
}

int getVAR( struct lex_t *lexem, char *word, int word_sz )
{
	check_pointer( lexem, 1 );
	check_pointer( word, 1 );

	for( int i = 0; i < word_sz; i++ )
		lexem -> value[ i ] = word[ i ];

	return 0;
}
