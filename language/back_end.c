#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex_analyser.h"
#include "./../tree_t/tree.h"
#include "./../log/log.h"
#include "back_end.h"

int	RAM_COUNTER = 0;
int	LABEL_COUNTER = 0;

int analyser_make_asm( struct analyser_t *analyser )
{
	FILE *dest = fopen( "asm", "w" );

	fprintf( dest, 	"call main\n"
			"end\n" );

	back_node( analyser, tree_get_root( analyser -> tree ), dest );
}

int back_node( struct analyser_t *analyser, struct tree_node_t *node, FILE *dest )
{
	struct lex_t *lexem = ( struct lex_t * )tree_get_elem( node );

	switch( lexem -> key )
	{
		case 1:
		{
			fprintf( dest, "push %lg\n", lexem -> koeff );

			break;
		}

		case 2:
		{
			#define case_oper( operatr, cmd )				\
				case operatr:						\
				{							\
					back_node( analyser, L( node ), dest );		\
											\
					back_node( analyser, R( L( node ) ), dest );	\
											\
					fprintf( dest, #cmd"\n" );			\
											\
					break;						\
				}

			switch( *lexem -> value )
			{
				case_oper( '+', add )
				case_oper( '-', sub )
				case_oper( '*', mul )
				case_oper( '/', div )
				
				case '=':
				{
					back_node( analyser, R( L( node ) ), dest );

					char *variable = ( (struct lex_t * )tree_get_elem( L( node ) ) ) -> value;

					for( int i = 0; i < RAM_COUNTER; i++ )
						if( !strcmp( analyser -> vars[ i ] -> name, variable ) )
							fprintf( dest, "pop [%d]\n", i );

					break;
				}
			}

			#undef case_oper

			break;
		}

		case 3:
		{
			if( *lexem -> value == '1' )
			{
				struct lex_t *fst_cond = ( struct lex_t * )tree_get_elem( L( L( node ) ) );
				check_pointer( fst_cond, 1 );

				#define if_oper( symb, cmd )						\
					if( *fst_cond -> value == symb )				\
					{								\
						back_node( analyser, L( L( L( node ) ) ), dest );	\
													\
						back_node( analyser, R( L( L( L( node ) ) ) ), dest );	\
													\
						fprintf( dest, #cmd" %d\n", LABEL_COUNTER );		\
					}

				if_oper( '<', jae )
				if_oper( '>', jbe )
				if_oper( '?', jne )

				#undef if_oper
			
				#define ne( symb )	\
					*fst_cond -> value != symb

				if( ne( '<' ) && ne( '>' ) && ne( '?' ) )
				{
					back_node( analyser, L( L( node ) ), dest );
				
					fprintf( dest, 	"push 0\n"
							"je %d\n", LABEL_COUNTER );
				}

				#undef ne

				back_node( analyser, R( L( node ) ), dest );

				fprintf( dest, "label %d\n", LABEL_COUNTER++ );

				break;
			}
		}

		case 4:
		{
			for( int i = 0; i < RAM_COUNTER; i++ )
				if( !strcmp( analyser -> vars[ i ] -> name, lexem -> value ) )
					fprintf( dest, "push [%d]\n", i );

			break;
		}

		case 6:
		{
			struct func_t *func = lexem -> func;

			if( func -> mode == call )
			{
				fprintf( dest, "call %s\n", lexem -> value );

				break;
			}

			fprintf( dest, "label %s\n", func -> name );

			back_node( analyser, L( node ), dest );

			fprintf( dest, "ret\n" );

			break;
		}

		case 7:
		{
			
		}

		case 8:
		{
			analyser -> vars[ RAM_COUNTER ] = ( struct var_t * )calloc( 1, sizeof( struct var_t ) );
			analyser -> vars[ RAM_COUNTER ] -> name = ( ( struct lex_t * )tree_get_elem( L( node ) ) ) -> value;

			analyser -> vars[ RAM_COUNTER ] -> adrs = RAM_COUNTER++;

			break;
		}
	}

	if( lexem -> key != 1 && lexem -> key != 4 && lexem -> key != 7 )
		if( tree_get_next( node, right ) )
			back_node( analyser, R( node ), dest );
}
