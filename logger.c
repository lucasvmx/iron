#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "logger.h"

int print(int type, const char *text, ...)
{
	int len = 0;
	va_list list;
	
	va_start(list,text);
	
	switch(type)
	{
		case WARNING:
			fprintf(stdout, ANSI_COLOR_YELLOW "[WARN] " ANSI_COLOR_RESET);
			break;
		
		case ERROR:
			fprintf(stdout, ANSI_COLOR_RED "[ERROR] " ANSI_COLOR_RESET);
			break;
		
		case INFO:
			fprintf(stdout, ANSI_COLOR_CYAN "[NOTICE] " ANSI_COLOR_RESET);
			break;
		
		case LOG:
			fprintf(stdout, ANSI_COLOR_MAGENTA "[LOG] " ANSI_COLOR_RESET);
			break;

		case OK:
			fprintf(stdout, ANSI_COLOR_GREEN "[SUCCESS] " ANSI_COLOR_RESET);
			break;

		case PROMPT:
			fprintf(stdout, ANSI_COLOR_YELLOW "[PROMPT] " ANSI_COLOR_RESET);
			break;
	}
	
	len = vfprintf(stdout, text, list);
    fprintf(stdout, "\n");
    va_end(list);
	
	return len;
}

void panic(const char *text, ...)
{
    va_list list;

    va_start(list, text);
    fprintf(stdout, ANSI_COLOR_RED "[PANIC] " ANSI_COLOR_RESET);
    vfprintf(stdout, text, list);
    fprintf(stdout, "\n");
    va_end(list);

    exit(1);
}

void jump_line(FILE *p)
{
	if(p != NULL)
		fprintf(p,"\n");
}
