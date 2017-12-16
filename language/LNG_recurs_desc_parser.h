#ifndef __LRDP_H__
#define __LRDP_H__

#include "lex_analyser.h"
#include "./../log/log.h"
#include "./../tree_t/tree.h"

#define LRDP_MAX_PARAM_COUNT 16

struct tree_node_t*	getFunc( struct analyser_t * );
struct tree_node_t*	getI( struct analyser_t * );
struct tree_node_t*	getAssn( struct analyser_t * );
struct tree_node_t*	getKw( struct analyser_t * );
struct tree_node_t*	getVar( struct analyser_t * );
struct tree_node_t*	getE( struct analyser_t * );
struct tree_node_t*	getT( struct analyser_t * );
struct tree_node_t*	getB( struct analyser_t * );
struct tree_node_t*	getP( struct analyser_t * );
struct tree_node_t*	getN( struct analyser_t * );
int			analyser_make_tree( struct analyser_t * );

#define check_syntax( symb )									\
do												\
{												\
	if( *analyser -> lexems[ analyser -> cur_pos++ ] -> value != symb )			\
	{											\
		printf( "Syntax error! lexems[ %zd ] != symb\n", analyser -> cur_pos - 1 );	\
												\
		return NULL;									\
	}											\
} while( 0 )

#endif /*__LRDP_H__*/
