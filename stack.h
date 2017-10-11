#ifndef _STACK_H_
#define _STACK_H_

typedef struct stack_t		stack_t;

stack_t* 	stack_construct( size_t elem_s );

enum error_t 	stack_destruct( stack_t *name );

#define stack( type, name )				\
stack_t *name;						\
name = stack_construct( sizeof( type ) );		\
if( name == NULL )					\
	printf( "ERROR WHILE CONSTRUCTING STACK\n\n" );

enum error_t	push( stack_t *name, const void* value );
void* 		top( stack_t *name );
void*		pop( stack_t *name );
enum error_t	resize( stack_t *name, size_t capacity_new );
size_t 		size( stack_t *name );
size_t 		capacity( stack_t *name );
int		empty( stack_t *name );
enum error_t	check_stack( stack_t *name);

#endif /*_STACK_H_*/
