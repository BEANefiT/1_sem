#ifdef DEF_CMD
DEF_CMD( PUSH,  1 )
DEF_CMD( ADD,   2 )
DEF_CMD( MUL,   3 )
DEF_CMD( DIV,   4 )
DEF_CMD( SUB,   5 )
DEF_CMD( OUT,   6 )
DEF_CMD( POPR,  7 )
DEF_CMD( PUSHR, 8 )
DEF_CMD( LABEL, 9 )
DEF_CMD( JMP, 	10 )
DEF_CMD( JE,	11 )
DEF_CMD( JNE,	12 )
DEF_CMD( JA,	13 )
DEF_CMD( JAE,	14 )
DEF_CMD( JB,	15 )
DEF_CMD( JBE,	16 )
#endif

#ifdef _Compile_
#define CMD_PUSH_code							\
do									\
{									\
	double value = 0;						\
	sscanf( src + src_cur, "%lg%n", &value, &src_cur_delta );	\
	src_cur += src_cur_delta;					\
	memcpy( exe_cur, &value, sizeof( double ) );			\
	exe_cur += sizeof( double );					\
} while( 0 )

#define REG_READ_code							\
do									\
{									\
	char *reg = ( char * )calloc( 2, sizeof( char ) );		\
	sscanf ( src + src_cur, "%s%n", reg, &src_cur_delta );		\
	src_cur += src_cur_delta;					\
									\
	int reg_num;							\
	if( strcmp( reg, "ax" ) == 0 )					\
		reg_num = 1;						\
	if( strcmp( reg, "bx" ) == 0 )					\
		reg_num = 2;						\
	if( strcmp( reg, "cx" ) == 0 )					\
		reg_num = 3;						\
	if( strcmp( reg, "dx" ) == 0 )					\
		reg_num = 4;						\
									\
	memcpy( exe_cur, &reg_num, sizeof( int ) );			\
	exe_cur += sizeof( int );					\
} while( 0 )

#define CMD_LABEL_code							\
do									\
{									\
	int label = -1;							\
	sscanf( src + src_cur, "%d%n", &label, &src_cur_delta );	\
	src_cur += src_cur_delta;					\
	labels[ label ] = ( size_t )exe_cur - ( size_t )exe;		\
} while( 0 )


#define CMD_JMP_code							\
do									\
{									\
	int label = -1;							\
	sscanf( src + src_cur, "%d%n", &label, &src_cur_delta );	\
	src_cur += src_cur_delta;					\
	memcpy( exe_cur, labels + label, sizeof( size_t ) );		\
	exe_cur += sizeof( size_t );					\
} while( 0 )

#endif /*__Compile__*/
