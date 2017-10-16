#ifndef _STACK_H_
#define _STACK_H_
//#define __DEBUG__
typedef struct stack_t		stack_t;

/**
*@brief
*
*@param elem_s
*
*@return
*/
stack_t* 	stack_construct( size_t elem_s );

/**
* @brief 
*
* @param name
*
* @return 
*/
enum error_t 	stack_destruct( stack_t *name );

/**
* @brief 
*
* @param type
* @param name
*
* @return 
*/
#define stack( type, name )				\
stack_t *name;						\
name = stack_construct( sizeof( type ) );		\
if( name == NULL )					\
	printf( "ERROR WHILE CONSTRUCTING STACK\n\n" );

/**
* @brief 
*
* @param name
* @param value
*
* @return 
*/
enum error_t	push( stack_t *name, const void* value );

/**
* @brief 
*
* @param name
* @param dest
*
* @return 
*/
enum error_t	top( stack_t *name, void* dest );

/**
* @brief 
*
* @param name
* @param dest
*
* @return 
*/
enum error_t	pop( stack_t *name, void* dest );

/**
* @brief 
*
* @param name
* @param capacity_new
*
* @return 
*/
enum error_t	resize( stack_t *name, size_t capacity_new );

/**
* @brief 
*
* @param name
*
* @return 
*/
size_t 		size( stack_t *name );

/**
* @brief 
*
* @param name
*
* @return 
*/
size_t 		capacity( stack_t *name );

/**
* @brief 
*
* @param name
*
* @return 
*/
int		empty( stack_t *name );

/**
* @brief 
*
* @param name
*
* @return 
*/
enum error_t	check_stack( stack_t *name);

/**
* @brief 
*
* @param name
*
* @return 
*/
size_t		hash( stack_t *name );

#endif /*_STACK_H_*/
