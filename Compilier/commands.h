#ifdef DEF_CMD
DEF_CMD( PUSH, push, 1, {
				int push_type = 0;
				void *arg = calloc( 1, sizeof( int ) );
				push_type();
				int cmd = CMD_PUSH + push_type;
				to_exe( &cmd, int );
				if( push_type != 0 )
				{
					to_exe( arg, int );
				}
				else
				{
					to_exe( arg, double );
				}
				free( arg );
			},

      2 * sizeof( int );{
				int push_type = 0;
				void *arg = calloc( 1, sizeof( int ) );
				push_type();
				free( arg );
				if( push_type == 0 )
					exe_cur += sizeof( int );
      			},

			{
				double value = 0;
				from_exe( &value, double );
				Push( values, &value );
			});
DEF_CMD( PUSHR, push, 2, , 0, {
				double value = 0;
				REG_read( value );
				Push( values, &value );
			      });
DEF_CMD( RAMPUSH, push, 3, , 0, {
					int index = 0;
					from_exe( &index, int );
					RAMPUSH_code();
				});
DEF_CMD( RAMPUSHR, push, 4, , 0, {
					int index = 0;
					REG_read( index );
					RAMPUSH_code();
				 });
DEF_CMD( POPR, pop, 5, {
				int push_type = 0;
				void *arg = calloc( 1, sizeof( int ) );
				push_type();
				int cmd = CMD_POPR + push_type - 1;
				to_exe( &cmd, int );
				to_exe( arg, int );
				free( arg );
			},

			2 * sizeof( int ),
			
			{
				double value = 0;
				Pop( values, &value );
				REG_write( value );
			});
DEF_CMD( RAMPOP, pop, 6, , 0, {
				int index = 0;
				from_exe( &index, int );
				double value = 0;
				Pop( values, &value );
				RAM_sz_check( index );
				( ( CPU ) -> RAM )[ index ] = value;
			      });
DEF_CMD( RAMPOPR, pop, 7, , 0, {
				double index = 0;
				REG_read( index );
				RAM_sz_check( index );
				double value = 0;
				Pop( values, &value );
				( ( CPU ) -> RAM )[ ( int )index ] = value;
			       });
DEF_CMD( ADD, add, 8,  { to_exe( &CMD_ADD, int ); }, sizeof( int ), binary_cmd( + ) );
DEF_CMD( MUL, mul, 9,  { to_exe( &CMD_MUL, int ); }, sizeof( int ), binary_cmd( * ) );
DEF_CMD( DIV, div, 10, { to_exe( &CMD_DIV, int ); }, sizeof( int ), binary_cmd( / ) );
DEF_CMD( SUB, sub, 11, { to_exe( &CMD_SUB, int ); }, sizeof( int ), binary_cmd( - ) );
DEF_CMD( OUT, out, 12, { to_exe( &CMD_OUT, int ); }, sizeof( int ), {
									double value = 0;
									Top( values, &value );
									printf( "%lg\n", value );
								    });
DEF_CMD( LABEL, label, 13, ,  0;
					int label = -1;
					from_src( %d, &label );
					labels[ label ] = ( size_t )exe_cur - ( size_t )exe;
				,

				{

				});
DEF_CMD( JMP, jmp, 14,  {
				to_exe( &CMD_JMP, int );
				CMD_JMP_code();
			},

			sizeof( int ) + sizeof( size_t ),
			
			{
				size_t tmp = 0;
				from_exe( &tmp, size_t );
				( CPU ) -> exe_cur =  ( CPU ) -> exe + tmp ;
			});
DEF_CMD( JE, je, 15,    {
				to_exe( &CMD_JE, int );
				CMD_JMP_code();
			},

			sizeof( int ) + sizeof( size_t ),
			
			{
				JmpIf( == );
			});
DEF_CMD( JNE, jne, 16,  {
				to_exe( &CMD_JNE, int );
				CMD_JMP_code();
			},

			sizeof( int ) + sizeof( size_t ),
			
			{
				JmpIf( != );
			});
DEF_CMD( JA, ja, 17,    {
				to_exe( &CMD_JA, int );
				CMD_JMP_code();
			},

			sizeof( int ) + sizeof( size_t ),
			
			{
				JmpIf( > );
			});
DEF_CMD( JAE, jae, 18,  {
				to_exe( &CMD_JAE, int );
				CMD_JMP_code();
			},

			sizeof( int ) + sizeof( size_t ),
			
			{
				JmpIf( >= );
			});
DEF_CMD( JB, jb, 19,    {
				to_exe( &CMD_JB, int );
				CMD_JMP_code();
			},

			sizeof( int ) + sizeof( size_t ),
			
			{
				JmpIf( < );
			});
DEF_CMD( JBE, jbe, 20,  {
				to_exe( &CMD_JBE, int );
				CMD_JMP_code();
			},

			sizeof( int ) + sizeof( size_t ),
			
			{
				JmpIf( <= );
			});
DEF_CMD( CALL, call, 21,{
				to_exe( &CMD_CALL, int );
				CMD_JMP_code();
			},

			sizeof( int ) + sizeof( size_t ),
			
			{
				size_t ret = ( size_t )(( CPU ) -> exe_cur) - ( size_t )(( CPU ) -> exe ) + sizeof( size_t );
				Push( rets, &ret );
				size_t tmp = 0;
				from_exe( &tmp, size_t );
				( CPU ) -> exe_cur = ( CPU ) -> exe + tmp;
			});
DEF_CMD( RET, ret, 22, { to_exe( &CMD_RET, int ); }, sizeof( int ), {
									size_t ret = 0;
									Pop( rets, &ret );
									( CPU ) -> exe_cur = ( CPU ) -> exe + ret;
								    });
DEF_CMD( SQRT, sqrt, 23, { to_exe( &CMD_SQRT, int ); }, sizeof( int ), unary_cmd( sqrt ) );
#endif /*DEF_CMD*/

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#ifdef _Compilier_
#define push_type()							\
do									\
{									\
	char *tmp = ( char * )calloc( 8, sizeof( char ) );		\
	char tmpc = 0;							\
	check_src( %c, &tmpc );						\
	while( tmpc == ' ' )						\
	{								\
		src_cur += 1;						\
		check_src( %c, &tmpc );					\
	}								\
	if( tmpc == '[' )						\
	{								\
		check_src( [%[^]]], tmp );				\
		if( ifreg( tmp ) )					\
		{							\
			push_type = 3;					\
			REG_READ_code();				\
		}							\
		else							\
		{							\
			push_type = 2;					\
			int index;					\
			src_cur += 1;					\
			from_src( %d, &index );				\
			memcpy( arg, &index, sizeof( int ) );		\
		}							\
	}								\
	else								\
	{								\
		check_src( %s, tmp );					\
		if( ifreg( tmp ) )					\
		{							\
			push_type = 1;					\
			REG_READ_code();				\
		}							\
		else							\
		{							\
			arg = realloc( arg, sizeof( double ) );		\
			double value = 0;				\
			from_src( %lg, &value );			\
			memcpy( arg, &value, sizeof( double ) );	\
		}							\
	}								\
	free( tmp );							\
} while( 0 )

#define REG_READ_code()							\
do									\
{									\
	int reg_num = 0;						\
	if( strcmp( tmp, "ax" ) == 0 )					\
		reg_num = 1;						\
	if( strcmp( tmp, "bx" ) == 0 )					\
		reg_num = 2;						\
	if( strcmp( tmp, "cx" ) == 0 )					\
		reg_num = 3;						\
	if( strcmp( tmp, "dx" ) == 0 )					\
		reg_num = 4;						\
	memcpy( arg, &reg_num, sizeof( int ) );				\
} while( 0 )

#define CMD_JMP_code()							\
do									\
{									\
	int label = -1;							\
	from_src( %d, &label );						\
	to_exe( labels + label, size_t );				\
} while( 0 )

#define ifreg( tmp ) strcmp( tmp, "ax" ) == 0 || strcmp( tmp, "bx" ) == 0 || strcmp( tmp, "cx" ) == 0 || strcmp( tmp, "dx" ) == 0

#define	to_exe( ptr, size )						\
do									\
{									\
	memcpy( exe_cur, ptr, sizeof( size ) );				\
	exe_cur += sizeof( size );					\
} while( 0 )

#define from_src( type, dest )						\
do									\
{									\
	sscanf( src + src_cur, #type"%n", dest, &src_cur_delta );	\
	src_cur += src_cur_delta;					\
} while( 0 )

#define check_src( type, dest )						\
do									\
{									\
	sscanf( src + src_cur, #type"%n", dest, &src_cur_delta );	\
} while( 0 )
#endif /*_Compilier_*/

#ifdef _CPU_

#define from_exe( dest, type )						\
do									\
{									\
	memcpy( dest, ( CPU ) -> exe_cur, sizeof( type ) );		\
	( CPU ) -> exe_cur += sizeof( type );				\
} while( 0 )

#define Push( stack_name, ptr )				\
do							\
{							\
	Do( push( ( CPU ) -> stack_name, ptr ) );	\
} while( 0 )

#define Pop( stack_name, ptr )				\
do							\
{							\
	Do( pop( ( CPU ) -> stack_name, ptr ) );	\
} while( 0 )

#define Top( stack_name, ptr )				\
do							\
{							\
	Do( top( ( CPU ) -> stack_name, ptr ) );	\
} while( 0 )

#define REG_read( reg )				\
do						\
{						\
	int reg_num = 0;			\
	from_exe( &reg_num, int );		\
	switch( reg_num )			\
	{					\
		case 1:				\
		{				\
			reg = ( CPU ) -> ax;	\
			break;			\
		}				\
		case 2:				\
		{				\
			reg = ( CPU ) -> bx;	\
			break;			\
		}				\
		case 3:				\
		{				\
			reg = ( CPU ) -> cx;	\
			break;			\
		}				\
		case 4:				\
		{				\
			reg = ( CPU ) -> dx;	\
			break;			\
		}				\
	}					\
} while( 0 )

#define REG_write( arg )			\
do						\
{						\
	int reg_num = 0;			\
	from_exe( &reg_num, int );		\
	switch( reg_num )			\
	{					\
		case 1:				\
		{				\
			( CPU ) -> ax = arg;	\
			break;			\
		}				\
		case 2:				\
		{				\
			( CPU ) -> bx = arg;	\
			break;			\
		}				\
		case 3:				\
		{				\
			( CPU ) -> cx = arg;	\
			break;			\
		}				\
		case 4:				\
		{				\
			( CPU ) -> dx = arg;	\
			break;			\
		}				\
	}					\
} while( 0 )

#define binary_cmd( operator )				\
do							\
{							\
	double value1 = 0, value2 = 0;			\
	Do( pop( ( CPU ) -> values, &value1 ) );	\
	Do( pop( ( CPU ) -> values, &value2 ) );	\
	double result = value2 operator value1;		\
	Do( push( ( CPU ) -> values, &result ) );	\
} while( 0 )

#define unary_cmd( operator )			\
do						\
{						\
	double value = 0;			\
	Pop( values, &value );			\
	double result = operator( value );	\
	Push( values, &result );		\
} while( 0 )

#define RAMPUSH_code()					\
do							\
{							\
	if( index >= ( CPU ) -> RAM_sz )		\
	{						\
							\
	}						\
	double value = ( ( CPU ) -> RAM )[ index ];	\
	Push( values, &value );				\
} while( 0 )

#define RAM_sz_check( size )										\
do													\
{													\
	if( size >= ( CPU ) -> RAM_sz )									\
	{												\
		( CPU ) -> RAM = ( double * )realloc( ( CPU ) -> RAM, ( size + 1 ) * sizeof( double ) );\
		( CPU ) -> RAM_sz = size + 1;								\
	}												\
} while( 0 )

#define JmpIf( operator )					\
do								\
{								\
	double value1 = 0, value2 = 0;				\
	Pop( values, &value1 );					\
	Pop( values, &value2 );					\
	if( value1 operator value2 )				\
	{							\
		size_t tmp = 0;					\
		from_exe( &tmp, size_t );			\
		( CPU ) -> exe_cur = ( CPU ) -> exe + tmp;	\
	}							\
	else							\
	{							\
		( CPU ) -> exe_cur += sizeof( size_t );		\
	}							\
} while( 0 )

#endif /*_CPU_*/
#endif /*_COMMANDS_H_*/
