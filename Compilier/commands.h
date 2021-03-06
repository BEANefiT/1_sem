#ifdef DEF_CMD

#define MAX_LABEL_LENGTH 64

DEF_CMD( PUSH, push, 1, {
				int push_type = 0;
				void *arg = calloc( 1, sizeof( int ) );
				push_type();
				int cmd = CMD_PUSH + push_type;
				to_exe( &cmd, int );
				/*if( push_type != 0 )
				{*/
					to_exe( arg, int );
				/*}
				else
				{
					to_exe( arg, double );
				}*/
				free( arg );
			},

      2 * sizeof( int ),

      0;{
				int push_type = 0;
				void *arg = calloc( 1, sizeof( int ) );
				push_type();
                switch (push_type)
                {
                    case 0:
                    {
                        elf_cur += 6;
                        break;
                    }

                    case 1:
                    {
                        elf_cur += 1;
                        break;
                    }

                    case 2:
                    {
                        elf_cur += 14;
                        break;
                    }

                    case 3:
                    {
                        elf_cur += 21;
                        break;
                    }
                }
				
                free( arg );
        },


			{
				int/*double*/ value = 0;
				from_exe( &value, int/*double*/ );
				Push( values, &value );
			});
DEF_CMD( PUSHR, push, 2, , 0, 0, {
				int/*double*/ value = 0;
				REG_read( value );
				Push( values, &value );
			      });
DEF_CMD( RAMPUSH, push, 3, , 0, 0, {
					int index = 0;
					from_exe( &index, int );
					RAMPUSH_code();
				});
DEF_CMD( RAMPUSHR, push, 4, , 0, 0, {
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
            
            0;{
				int push_type = 0;
				void *arg = calloc( 1, sizeof( int ) );
				push_type();
                switch (push_type)
                {
                    case 0:
                    {
                        break;
                    }

                    case 1:
                    {
                        elf_cur += 1;
                        break;
                    }

                    case 2:
                    {
                        elf_cur += 14;
                        break;
                    }

                    case 3:
                    {
                        elf_cur += 22;
                        break;
                    }
                }
				
                free( arg );
              },
			
			{
				int/*double*/ value = 0;
				Pop( values, &value );
				REG_write( value );
			});
DEF_CMD( RAMPOP, pop, 6, , 0, 0, {
				int index = 0;
				from_exe( &index, int );
				int/*double*/ value = 0;
				Pop( values, &value );
				RAM_sz_check( index );
				( ( CPU ) -> RAM )[ index ] = value;
			      });
DEF_CMD( RAMPOPR, pop, 7, , 0, 0, {
				int/*double*/ index = 0;
				REG_read( index );
				RAM_sz_check( index );
				int/*double*/ value = 0;
				Pop( values, &value );
				( ( CPU ) -> RAM )[ ( int )index ] = value;
			       });
DEF_CMD( ADD, add, 8,  { to_exe( &CMD_ADD, int ); }, sizeof( int ), 6, binary_cmd( + ) );
DEF_CMD( MUL, mul, 9,  { to_exe( &CMD_MUL, int ); }, sizeof( int ), 6, binary_cmd( * ) );
DEF_CMD( DIV, div, 10, { to_exe( &CMD_DIV, int ); }, sizeof( int ), 6, binary_cmd( / ) );
DEF_CMD( SUB, sub, 11, { to_exe( &CMD_SUB, int ); }, sizeof( int ), 6, binary_cmd( - ) );
DEF_CMD( OUT, out, 12, { to_exe( &CMD_OUT, int ); }, sizeof( int ), 74, {
									int/*double*/ value = 0;
									Pop( values, &value );
									printf( "%d\n", value );
								    });
DEF_CMD( LABEL, label, 13, ,  0;
					char *label = ( char * )calloc( MAX_LABEL_LENGTH, sizeof( char ) );

					from_src( %s, label );

					struct labels_t *newlabel =
						( struct labels_t * )calloc( 1, sizeof( struct labels_t ) );

					newlabel -> name =
						( char * )calloc( strlen( label ), sizeof( char ) );

					strcpy( newlabel -> name, label );
					newlabel -> pos =
						( long )exe_cur - ( long )( Compilier -> exe );

                    newlabel -> elf_offs = elf_cur;

					Compilier -> labels[ Compilier -> lbl_count++ ] = newlabel;
				,
            
                0,

				{

				});
DEF_CMD( JMP, jmp, 14,  {
				to_exe( &CMD_JMP, int );
				CMD_JMP_code();
			},

			sizeof( int ) + 2 * sizeof( int ),
			
            2,

			{
				int tmp = 0;
				from_exe( &tmp, int );
                CPU -> exe_cur += 4;
				( CPU ) -> exe_cur =  ( CPU ) -> exe + tmp ;
			});
DEF_CMD( JE, je, 15,    {
				to_exe( &CMD_JE, int );
				CMD_JMP_code();
			},

			sizeof( int ) + 2 * sizeof( int ),
			
            7,

			{
				JmpIf( == );
			});
DEF_CMD( JNE, jne, 16,  {
				to_exe( &CMD_JNE, int );
				CMD_JMP_code();
			},

			sizeof( int ) + 2 * sizeof( int ),
			
            7,

			{
				JmpIf( != );
			});
DEF_CMD( JA, ja, 17,    {
				to_exe( &CMD_JA, int );
				CMD_JMP_code();
			},

			sizeof( int ) + 2 * sizeof( int ),
			
            7,

			{
				JmpIf( > );
			});
DEF_CMD( JAE, jae, 18,  {
				to_exe( &CMD_JAE, int );
				CMD_JMP_code();
			},

			sizeof( int ) + 2 * sizeof( int ),
			
            7,

			{
				JmpIf( >= );
			});
DEF_CMD( JB, jb, 19,    {
				to_exe( &CMD_JB, int );
				CMD_JMP_code();
			},

			sizeof( int ) + 2 * sizeof( int ),
			
            7,

			{
				JmpIf( < );
			});
DEF_CMD( JBE, jbe, 20,  {
				to_exe( &CMD_JBE, int );
				CMD_JMP_code();
			},

			sizeof( int ) + 2 * sizeof( int ),
			
            7,

			{
				JmpIf( <= );
			});
DEF_CMD( CALL, call, 21,{
				to_exe( &CMD_CALL, int );
				CMD_JMP_code();
			},

			sizeof( int ) + 2 * sizeof( int ),
			
            5,

			{
				int ret = ( long )(( CPU ) -> exe_cur) - ( long )(( CPU ) -> exe ) + 2 * sizeof( int );
                Push( values, &ret);
				Push( rets, &ret );
				size_t tmp = 0;
				from_exe( &tmp, int );
                CPU -> exe_cur += 4;
				( CPU ) -> exe_cur = ( CPU ) -> exe + tmp;
			});
DEF_CMD( RET, ret, 22, { to_exe( &CMD_RET, int ); }, sizeof( int ), 1, {
									size_t ret = 0;
                                    Pop( values, &ret );
									Pop( rets, &ret );
									( CPU ) -> exe_cur = ( CPU ) -> exe + ret;
								    });
DEF_CMD( SQRT, sqrt, 23, { to_exe( &CMD_SQRT, int ); }, sizeof( int ), 0, unary_cmd( sqrt ) );
DEF_CMD( IN, in, 24, { to_exe( &CMD_IN, int ); }, sizeof( int ), 0, {
									int/*double*/ value = 0;
									scanf( "%d", &value );
									Push( values, &value );
								 });
DEF_CMD( SIN, sin, 25, { to_exe( &CMD_SIN, int ); }, sizeof( int ), 0, unary_cmd( sin ) );
DEF_CMD( COS, cos, 26, { to_exe( &CMD_COS, int ); }, sizeof( int ), 0, unary_cmd( cos ) );
DEF_CMD( TG, tan, 27, { to_exe( &CMD_TG, int ); }, sizeof( int ), 0, unary_cmd( tan ) );
DEF_CMD( CAT, cat, 28, { to_exe( &CMD_CAT, int ); }, sizeof( int ), 0, DrawCat(); );
DEF_CMD( END, end, 29, { to_exe( &CMD_END, int ); }, sizeof( int ), 17, return 0 );
#endif /*DEF_CMD*/

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#ifdef _Compilier_
#define push_type()							            \
do									                    \
{									                    \
	char *tmp = ( char * )calloc( /*8*/4, sizeof( char ) );  \
	char tmpc = 0;							            \
	check_src( %c, &tmpc );						        \
	while( tmpc == ' ' )						        \
	{								                    \
		src_cur += 1;						            \
		check_src( %c, &tmpc );					        \
	}								                    \
	if( tmpc == '[' )						            \
	{								                    \
		check_src( [%[^]]], tmp );				        \
		if( ifreg( tmp ) )					            \
		{							                    \
			push_type = 3;					            \
			REG_READ_code();				            \
		}							                    \
		else							                \
		{							                    \
			push_type = 2;					            \
			int index;					                \
			src_cur += 1;					            \
			from_src( %d, &index );				        \
			memcpy( arg, &index, sizeof( int ) );		\
		}							                    \
	}								                    \
	else								                \
	{								                    \
		check_src( %s, tmp );					        \
		if( ifreg( tmp ) )					            \
		{							                    \
			push_type = 1;					            \
			REG_READ_code();				            \
		}							                    \
		else							                \
		{							                    \
			/*arg = realloc( arg, sizeof( double ) );	    \
			double*/int value = 0;				            \
			from_src( %d/*lg*/, &value );			        \
			memcpy( arg, &value, sizeof( int ) );/*double ) );*/   \
		}							                    \
	}								                    \
	free( tmp );							            \
} while( 0 )

#define REG_READ_code()							\
do									\
{									\
	int reg_num = 0;						\
	if( strcmp( tmp, "ax" ) == 0 )					\
		reg_num = 0;						\
	if( strcmp( tmp, "bx" ) == 0 )					\
		reg_num = 3;						\
	if( strcmp( tmp, "cx" ) == 0 )					\
		reg_num = 1;						\
	if( strcmp( tmp, "dx" ) == 0 )					\
		reg_num = 2;						\
	memcpy( arg, &reg_num, sizeof( int ) );				\
} while( 0 )

#define CMD_JMP_code()								                    \
do										                                \
{										                                \
	char *label = ( char * )calloc( MAX_LABEL_LENGTH, sizeof( char ) ); \
	from_src( %s, label );							                    \
										                                \
	for( int i = 0; i < Compilier -> lbl_count; i++ )			        \
	{									                                \
		if( !strcmp( Compilier -> labels[ i ] -> name, label ) )	    \
		{								                                \
			to_exe( &( Compilier -> labels[ i ] -> pos ), int );	    \
            to_exe( &( Compilier -> labels[ i ] -> elf_offs), int);     \
										                                \
			break;							                            \
		}								                                \
	}									                                \
} while( 0 )

#define ifreg( tmp ) strcmp( tmp, "ax" ) == 0 || strcmp( tmp, "bx" ) == 0 || strcmp( tmp, "cx" ) == 0 || strcmp( tmp, "dx" ) == 0

#define	to_exe( ptr, size )						\
do									\
{									\
	memcpy( exe_cur, ptr, sizeof( size ) );				\
	exe_cur += sizeof( size );					\
} while( 0 )

#define from_src( type, dest )							\
do										\
{										\
	sscanf( Compilier -> src + src_cur, #type"%n", dest, &src_cur_delta );	\
	src_cur += src_cur_delta;						\
} while( 0 )

#define check_src( type, dest )							\
do										\
{										\
	sscanf( Compilier -> src + src_cur, #type"%n", dest, &src_cur_delta );	\
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

#define REG_read( reg )					\
do							\
{							\
	int reg_num = 0;				\
	from_exe( &reg_num, int );			\
	reg = ( ( CPU ) -> registers )[ reg_num ];	\
} while( 0 )

#define REG_write( arg )				\
do							\
{							\
	int reg_num = 0;				\
	from_exe( &reg_num, int );			\
	( ( CPU ) -> registers )[ reg_num ] = arg;	\
} while( 0 )

#define binary_cmd( operator )				\
do							\
{							\
	/*double*/int value1 = 0, value2 = 0;			\
	Do( pop( ( CPU ) -> values, &value1 ) );	\
	Do( pop( ( CPU ) -> values, &value2 ) );	\
	/*double*/int result = value2 operator value1;		\
	Do( push( ( CPU ) -> values, &result ) );	\
} while( 0 )

#define unary_cmd( operator )			\
do						\
{						\
	/*double*/int value = 0;			\
	Pop( values, &value );			\
	/*double*/int result = operator( value );	\
	Push( values, &result );		\
} while( 0 )

#define RAMPUSH_code()					\
do							\
{							\
	if( index >= ( CPU ) -> RAM_sz )		\
	{						\
							\
	}						\
	/*double*/int value = ( ( CPU ) -> RAM )[ index ];	\
	Push( values, &value );				\
} while( 0 )

#define RAM_sz_check( size )										\
do													\
{													\
	if( size >= ( CPU ) -> RAM_sz )									\
	{												\
		( CPU ) -> RAM = ( /*double*/int * )realloc( ( CPU ) -> RAM, ( size + 1 ) * sizeof( int/*double*/ ) );\
		( CPU ) -> RAM_sz = size + 1;								\
	}												\
} while( 0 )

#define JmpIf( operator )					        \
do								                    \
{								                    \
	int/*double*/ value1 = 0, value2 = 0;		    \
	Pop( values, &value1 );					        \
	Pop( values, &value2 );					        \
	if( value2 operator value1 )				    \
	{							                    \
		size_t tmp = 0;					            \
		from_exe( &tmp, size_t );			        \
        CPU -> exe_cur += 4;                        \
		( CPU ) -> exe_cur = ( CPU ) -> exe + tmp;	\
	}							                    \
	else							                \
	{							                    \
		( CPU ) -> exe_cur += sizeof( size_t );		\
	}							                    \
} while( 0 )

#define DrawCat()	                                                    \
do			                                                            \
{			                                                            \
	printf( "             *     ,MMM8&&&.            *\n"               \
            "                  MMMM88&&&&&    .\n"                      \
            "                 MMMM88&&&&&&&\n"                          \
            "     *           MMM88&&&&&&&&\n"                          \
            "                 MMM88&&&&&&&&\n"                          \
            "                 'MMM88&&&&&&'\n"                          \
            "                   'MMM8&&&'      *\n"                     \
            "          |\\___/|\n"                                      \
            "          )     (             .              '\n"          \
            "         =\\     /=\n"                                     \
            "           )===(       *\n"                                \
            "          /     \\\n"                                      \
            "          |     |\n"                                       \
            "         /       \\\n"                                     \
            "         \\       /\n"                                     \
            "  _/\\_/\\_/\\__  _/_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_/\\_\n"\
            "  |  |  |  |( (  |  |  |  |  |  |  |  |  |  |\n"           \
            "  |  |  |  | ) ) |  |  |  |  |  |  |  |  |  |\n"           \
            "  |  |  |  |(_(  |  |  |  |  |  |  |  |  |  |\n"           \
            "  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |\n" );        \
} while( 0 )

#endif /*_CPU_*/
#endif /*_COMMANDS_H_*/
