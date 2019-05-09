#include "log.h"
#include <stdio.h>
#include <stdarg.h>

static int bold = 0;
const char* red 		= "31";
const char* green 	= "32";
const char* yellow 	= "33";
const char* blue 		= "34";
const char* magenta = "35";
const char* cyan 		= "36";

void log_bold(){
	bold = 1;
}

void log_color(const char* color){
	printf("\033[%i;%sm", bold, color);
}

void log_reset(){
	printf("\033[0m");
}

void log_info(const char* fmt, ...){
	log_bold();
	log_color(blue);
	printf("[info] ");
	log_reset();

	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

void log_error(const char* fmt, ...){
	log_bold();
	log_color(red);
	printf("[error] ");
	log_reset();

	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

void log_warning(const char* fmt, ...){
	log_bold();
	log_color(yellow);
	printf("[warning] ");
	log_reset();

	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}
