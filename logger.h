
#ifndef LOGGER_HEADER
#define LOGGER_HEADER

// from https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_COLOR_RED     "\x1b[1;31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_YELLOW  "\x1b[1;33m"
#define ANSI_COLOR_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA "\x1b[1;35m"
#define ANSI_COLOR_CYAN    "\x1b[1;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// For use with rooter_printf
#define WARNING	1
#define ERROR	2
#define INFO 	3
#define LOG		4
#define OK		5
#define PROMPT	6
#define PANIC   7

int print(int type, const char *text, ...);
void panic(const char *text, ...);

#endif
