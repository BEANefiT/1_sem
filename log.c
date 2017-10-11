#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "log.h"

static FILE *log;

void log_constr()
{
	log = fopen( "log.txt", "w" );
	if (log == NULL) {
	}
}

void log_destr()
{
	fclose (log);
}

void print_log( char *msg, ... )
{
	va_list args;
	va_start( args, msg );
	fprintf( log, va_arg( msg, args ) );
	va_end( args );
}

