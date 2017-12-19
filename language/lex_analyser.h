#ifndef __LEX_ANALYSER_H__
#define __LEX_ANALYSER_H__

#include "./../tree_t/tree.h"

#define LEX_ANALYSER_MAX_VALUE_LENGTH 64
#define MAX_LEXEMS_COUNT 8192
#define RAM_SZ 1028 	//one more define of RAM_SZ in CPU.c
#define MAX_VAR_LENGTH 16

enum key_t
{
	val     = 1,
	oper    = 2,
	kw      = 3,
	var     = 4,
	br      = 5,
	func    = 6,
	cond	= 7,
	dclr	= 8
};

#define DEF_KW( kw, num, conds ) \
	kw = num,

enum kw_t
{
	#include "kwrds.h"
};

#undef DEF_KW

struct lex_t
{
	enum key_t	key;
	char		value[ LEX_ANALYSER_MAX_VALUE_LENGTH ];
	double		koeff;
	struct func_t*	func;
};

struct var_t
{
	char*	name;
	int	adrs;
};

struct analyser_t
{
	struct lex_t*    lexems[ MAX_LEXEMS_COUNT ];
	struct var_t*    vars[ RAM_SZ ];
	struct tree_t*   tree;
	char*            src;
	size_t           lex_num;
	size_t           src_sz;
	size_t           cur_pos;
};

enum func_act
{
	call,
	decl
};

struct func_t
{
	struct lex_t**	params_arr;
	char*		name;
	int		param_count;
	enum func_act	mode;
};

int	analyser_constr( struct analyser_t *analyser );
int	analyser_make_lexems( struct analyser_t *analyser );
int	getKW( struct lex_t *lexem, char *word, int word_sz );
int	getFUNC( struct lex_t *lexem, char *word, int word_sz );
int	getVAR( struct lex_t *lexem, char *word, int word_sz );
char*	getVAL( char *src, struct lex_t *lexem );
char*	getOPER( char *src, struct lex_t *lexem );
char*	getBR( char *src, struct lex_t *lexem );
int	getDEF( struct lex_t *lexem, char *word, int word_sz );

#define check_for_lexem( ARG, key_num )							\
if( is##ARG )										\
{											\
	struct lex_t *lexem = ( struct lex_t * )calloc( 1, sizeof( struct lex_t ) );	\
	check_pointer( lexem, 1 );							\
											\
	lexem -> key = key_num;								\
											\
	src = get##ARG ( src, lexem );							\
	check_pointer( src, 1 );							\
											\
	analyser -> lexems[ analyser -> lex_num ++ ] = lexem;				\
											\
	continue;									\
}

#define check_for_word( ARG, key_num )							\
if( is##ARG )										\
{											\
	struct lex_t *lexem = ( struct lex_t * )calloc( 1, sizeof( struct lex_t ) );	\
	check_pointer( lexem, 1 );							\
											\
	lexem -> key = key_num;								\
											\
	get##ARG ( lexem, word, word_sz );						\
											\
	analyser -> lexems[ analyser -> lex_num ++ ] = lexem;				\
											\
	continue;									\
}

#define isTRASH \
	elem == ' ' || elem == '\n' || elem == '\t' || elem == '\v' || elem == ',' || elem == ';'

#define isDEF \
	!strcmp( word, "def" )


#define isVAL \
'0' <= elem && elem <= '9'


#define isOPER \
elem == '+' || elem == '-' || elem == '/' || elem == '*' || elem == '=' || elem == '>' || elem == '<' || elem == '?'


#define isBR \
elem == '(' || elem == ')' || elem == '[' || elem == ']' || elem == '{' || elem == '}'


#define isLETTER ( 'a' <= elem && 'z' >= elem ) || ( 'A' <= elem && 'Z' >= elem )


#define isKW \
!( strcmp( word, "esli" ) * strcmp( word, "inache" ) * strcmp( word, "poka" ) * strcmp( word, "dlya" ) )


#define isFUNC \
*src == '{'


#define isVAR \
1 == 1 			// just to kill if()

#endif /*__LEX_ANALYSER_H__*/
