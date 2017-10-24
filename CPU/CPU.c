#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./../stack_t/stack.h"
#include "./../log/log.h"
#include "CPU.h"

int main()
{
	log_constr();
	struct CPU_structure CPU;
	CPU_construct( &CPU );
	run( &CPU );
	log_destr();
	return 0;
}

int CPU_construct( struct CPU_structure *CPU )
{
	stack( double, values );
	( CPU ) -> values = values;
	( CPU ) -> exe_sz = 0;
	( CPU ) -> exe = getcode( CPU );
}

int run( struct CPU_structure *CPU )
{
	#define DEF_CMD( name, num ) name = num,
	enum CMD
	{
		#include "./../Compilier/commands.h"
		end = 0
	};
	#undef DEF_CMD
	while( ( CPU ) -> exe_cur < ( CPU ) -> exe + ( CPU ) -> exe_sz )
	{
		int cmd = 0;
		memcpy( &cmd, ( CPU ) -> exe_cur, sizeof( int ) );
		( ( CPU ) -> exe_cur ) += sizeof( int );
		switch( cmd )
		{
			case PUSH:
			{
				double value = 0;
				memcpy( &value, ( CPU ) -> exe_cur, sizeof( double ) );
				( CPU ) -> exe_cur += sizeof( double );
				Do( push( ( CPU ) -> values, &value ) );
				break;
			}

			case ADD:
			{
				double value1 = 0, value2 = 0;
				Do( pop( ( CPU ) -> values, &value1 ) );
				Do( pop( ( CPU ) -> values, &value2 ) );
				double result = value1 + value2;
				Do( push( ( CPU ) -> values, &result ) );
				break;
			}

			case MUL:
			{
				double value1 = 0, value2 = 0;
				Do( pop( ( CPU ) -> values, &value1 ) );
				Do( pop( ( CPU ) -> values, &value2 ) );
				double result = value1 * value2;
				Do( push( ( CPU ) -> values, &result ) );
				break;
			}

			case DIV:
			{
				double value1 = 0, value2 = 0;
				Do( pop( ( CPU ) -> values, &value2 ) );
				Do( pop( ( CPU ) -> values, &value1 ) );
				double result = value1 / value2;
				Do( push( ( CPU ) -> values, &result ) );
				break;
			}
			
			case SUB:
			{
				double value1 = 0, value2 = 0;
				Do( pop( ( CPU ) -> values, &value2 ) );
				Do( pop( ( CPU ) -> values, &value1 ) );
				double result = value1 - value2;
				Do( push( ( CPU ) -> values, &result ) );
				break;
			}
			case OUT:
			{
				double value = 0;
				Do( top( ( CPU ) -> values, &value ) );
				printf( "%lg\n", value );
				break;
			}
			case POPR:
			{
				double value = 0;
				Do( pop( ( CPU ) -> values, &value ) );
				int reg_num = 0;
				memcpy( &reg_num, ( CPU ) -> exe_cur, sizeof( int ) );
				( CPU ) -> exe_cur += sizeof( int );
				
				switch( reg_num )
				{
					case 1:
					{
						( CPU ) -> ax = value;
						break;
					}
					case 2:
					{
						( CPU ) -> bx = value;
						break;
					}
					case 3:
					{
						( CPU ) -> cx = value;
						break;
					}
					case 4:
					{
						( CPU ) -> dx = value;
						break;
					}
				}

				break;

			}
			case PUSHR:
			{
				int reg_num = 0;
				memcpy( &reg_num, ( CPU ) -> exe_cur, sizeof( int ) );
				( CPU ) -> exe_cur += sizeof( int );

				switch( reg_num )
				{
					case 1:
					{
						Do( push( ( CPU ) -> values, &(( CPU ) -> ax )) );
						break;
					}
					case 2:
					{
						Do( push( ( CPU ) -> values, &(( CPU ) -> bx )) );
						break;
					}
					case 3:
					{
						Do( push( ( CPU ) -> values, &(( CPU ) -> cx )) );
						break;
					}
					case 4:
					{
						Do( push( ( CPU ) -> values, &(( CPU ) -> dx )) );
						break;
					}
				}

				break;
			}
			case JMP:
			{
				size_t tmp = 0;
				memcpy( &tmp, ( CPU ) -> exe_cur, sizeof( size_t ) );
				( CPU ) -> exe_cur =  ( CPU ) -> exe + tmp ;
				break;
			}
		}
	}

}

void *getcode( struct CPU_structure *CPU )
{
	FILE *exe_file = fopen( "exe", "r" );
	( CPU ) -> exe_sz = fSize( exe_file );
	void *exe = calloc( ( CPU ) -> exe_sz, sizeof( char ) );
	fread( exe, sizeof( char ), ( CPU ) -> exe_sz, exe_file );
	fclose( exe_file );
	( CPU ) -> exe_cur = exe;
	return exe;
}

size_t fSize( FILE *file_name )
{
	fseek( file_name, 0, SEEK_END );
	size_t file_sz = ftell( file_name );
	rewind( file_name );
	return file_sz;
}
