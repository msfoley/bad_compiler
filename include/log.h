#ifndef LOG_H
#define LOG_H

int log_init(const char *log_file_, int verbose, int quiet);
void log_exit();

int print_info(const char *fmt, ...);
int print_debug(const char *fmt, ...);
int print_error(const char *fmt, ...);

#endif
