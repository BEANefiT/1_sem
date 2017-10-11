#ifndef _LOG_H_
#define _LOG_H_

#define ERROR( error )							\
do									\
{									\
	if( error == noerror )						\
		break;							\
	if( error == BAD_ARG )						\
	{								\
		print_log( "\t\tprocess returned BAD_ARG\n" );		\
		error = noerror;					\
	}								\
	if( error == EMPTY_STACK )					\
	{								\
		print_log( "\t\tprocess returned EMPTY_STACK\n" );	\
		error = noerror;					\
	}								\
	if( error == UNEXPECTED )					\
	{								\
		print_log( "\t\tprocess returned UNEXPECTED\n" );	\
		error = noerror;					\
	}								\
	if( error == FATAL )						\
	{								\
		print_log( "\t\tprocess returned FATAL\n" );		\
		error = noerror;					\
	}								\
} while( 0 )

#define Do( func )                            	\
do                                		\
{        					\
	print_log( "doing '"#func"'\n" );	\
	func;                      		\
  	if( error == noerror )         		\
      		print_log( "ok\n" );		\
	else					\
		ERROR( error );			\
} while( 0 )

enum error_t
{
	noerror,
	BAD_ARG,
	EMPTY_STACK,
	UNEXPECTED,
	FATAL
};

enum log_err_t
{
	SUCCESS,
	CANNOT_PRINT,
	NO_LOG_FILE
};

static enum error_t error = noerror;

int log_constr();
int log_destr();

#ifdef __GNUC__
	int print_log( char *msg, ... ) __attribute__(( format(printf, 1, 2) ));
#else
	int print_log( char *msg, ... );
#endif /*__GNUC__*/


#endif /*_LOG_H_*/
