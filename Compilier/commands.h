#ifdef DEF_CMD
DEF_CMD( PUSH, 1 )
DEF_CMD( ADD,  2 )
DEF_CMD( MUL,  3 )
DEF_CMD( DIV,  4 )
DEF_CMD( SUB,  5 )
#endif

#ifdef __Compile__

#define CMD_PUSH_code							\
do									\
{									\
	double value;							\
	sscanf( src + src_cur, "%lg%n", &value, &src_cur_delta );	\
	src_cur += src_cur_delta;					\
	memcpy( exe_cur, &value, sizeof( double ) );			\
	exe_cur += sizeof( double );					\
} while( 0 )

#endif /*__Compile__*/
