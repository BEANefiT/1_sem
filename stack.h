#ifndef _STACK_H_
#define _STACK_H_

typedef struct stack_t		stack_t;

enum elem_type
{
	INT,
	LONG,
	DOUBLE,
	FLOAT,
	CHAR
};

stack_t* stack_construct( size_t elem_s );

/*#define ERROR( error )
do
{
	if( error ==

} while( 0 )
*/
#define stack( type, name )				\
stack_t *name;						\
							\
enum elem_type name##_type = type;				\
size_t elem_s = 0;					\
if( name##_type == INT )				\
	elem_s = sizeof( int );				\
if( name##_type == LONG )				\
	elem_s = sizeof( long );			\
if( name##_type == DOUBLE )				\
	elem_s = sizeof( double );			\
if( name##_type == FLOAT )				\
	elem_s = sizeof( float );			\
if( name##_type == CHAR )				\
	elem_s = sizeof( char );			\
							\
name = stack_construct( elem_s );

enum error_t	push( stack_t* name, const void* value );
void* 	top( stack_t* name );
void*	pop( stack_t* name );
enum error_t	resize( stack_t* name, size_t capacity_new );
size_t 	size( stack_t* name );
size_t 	capacity( stack_t* name );
int	empty( stack_t* name );

#endif /*_STACK_H_*/
