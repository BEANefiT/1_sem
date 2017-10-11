#ifndef _LOG_H_
#define _LOG_H_

#define Do( func )                            	\
do                                		\
{        					\
	print_log( "doing '"#func"'\n" );	\
	func;                      		\
  	if( error == noerror )         		\
      		print_log( "ok\n" );   		\
} while( 0 )

enum error_t
{
	noerror,
	BAD_ARG,
	EMPTY_STACK,
	UNEXPECTED,
	FATAL
};

static enum error_t error = noerror;

void log_constr();
void log_destr();
void print_log( char *msg, ... );
#endif
