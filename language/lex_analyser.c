#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex_analyser.h"
#include "./../log/log.h"

int analyser_constr( struct analyser_t *analyser )
{
	analyser -> tree       = NULL;
	analyser -> src        = NULL;
	analyser -> lex_num    = 0;
	analyser -> src_sz     = 0;
	analyser -> cur_pos    = 0;
}

int analyser_make_lexems( struct analyser_t *analyser )
{
	check_pointer( analyser, 1 );

	char *src = analyser -> src;
	check_pointer( src, 1 );

	struct lex_t **lexems = ( struct lex_t ** )calloc( MAX_LEXEMS_COUNT, sizeof( struct lex_t * ) );
	check_pointer( lexems, 1 );
	
	while( src - analyser -> src < analyser -> src_sz )
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

		check_for_lexem( Punc, 10 )
		
		char *word = ( char * )calloc( LEX_ANALYSER_MAX_VALUE_LENGTH, sizeof( char ) );
		check_pointer( word, 1 );

		int word_sz = 0;

		while( isLETTER || isVAL )
		{
			word[ word_sz++ ] = *(src++);
			elem = *src;
		}

		word[ word_sz++ ] = '\0';

		if( !word_sz )
		{
			printf( "Syntax error: unrecognized symbol '%c'\n", elem );

			return 1;
		}

		word = ( char * )realloc( word, word_sz * sizeof( char ) );

		check_for_word( RET, 9 )
		
		check_for_word( OUT, 11 )

		check_for_word( IN, 12 )

		check_for_word( SQRT, 13 )

		check_for_word( KW,   3 )

		check_for_word( DEF, 8 )
		
		check_for_word( FUNC, 6 )

		check_for_word( VAR,  4 )
	}

	return 0;
}

char *getVAL( char *src, struct lex_t *lexem )
{
	check_pointer( lexem, NULL );
	check_pointer( src, NULL );

	size_t delta_cur = 0;

	sscanf( src, "%lg%n", &( lexem -> koeff ), &delta_cur );

	src += delta_cur;

	return src;
}

int getDEF( struct lex_t *lexem, char *word, int word_sz )
{
	check_pointer( lexem, 1 );
	check_pointer( word, 1 );

	return 0;
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

char *getPunc( char *src, struct lex_t *lexem )
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

	#define DEF_KW( name, num, conds )		\
		if( !strcmp( word, #name ) )		\
			*lexem -> value = num + '0';

	#include "kwrds.h"

	#undef DEF_KW

	return 0;
}

int getRET( struct lex_t *lexem, char *word, int word_sz )
{
}

int getOUT( struct lex_t *lexem, char *word, int word_sz )
{
}

int getIN( struct lex_t *lexem, char *word, int word_sz )
{
}

int getSQRT( struct lex_t *lexem, char *word, int word_sz )
{
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
