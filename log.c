#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "log.h"

static FILE *log;

int log_constr()
{
	log = fopen( "log.txt", "w" );
	if (log == NULL) {
		return NO_LOG_FILE;
	}
	return SUCCESS;
}

int log_destr()
{
	fclose (log);
	return SUCCESS;
}

int print_log( char *msg, ... )
{
	if( msg == NULL )
		return CANNOT_PRINT;
	va_list args;
	va_start( args, msg );
	vfprintf( log, msg, args );
	va_end( args );
}

