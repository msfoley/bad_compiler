#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#include <log.h>

FILE *log_file = NULL;
int verbose_flag = 0;
int quiet_flag = 0;

int log_init(const char *log_file_, int verbose, int quiet) {
    if (log_file_) {
        if (log_file) {
            fclose(log_file);
        }

        log_file = fopen(log_file_, "w");
    }

    if (verbose && quiet) {
        verbose = 0;
    }

    verbose_flag = verbose;
    quiet_flag = quiet;
}

void log_exit() {
    if (log_file) {
        fclose(log_file);
    }
}

int print_info(const char *fmt, ...) {
    FILE *f = log_file;
    va_list args;
    int ret;

    if (!fmt) {
        return -EINVAL;
    }

    if (quiet_flag) {
        return 0;
    }

    if (!f) {
        f = stdout;
    }

    va_start(args, fmt);
    ret = vfprintf(f, fmt, args);
    va_end(args);

    return ret;
}

int print_debug(const char *fmt, ...) {
    FILE *f = log_file;
    va_list args;
    int ret;

    if (!fmt) {
        return -EINVAL;
    }

    if (!verbose_flag) {
        return 0;
    }

    if (!f) {
        f = stdout;
    }

    va_start(args, fmt);
    ret = vfprintf(f, fmt, args);
    va_end(args);

    return ret;
}

int print_error(const char *fmt, ...) {
    FILE *f = log_file;
    va_list args;
    int ret;

    if (!fmt) {
        return -EINVAL;
    }

    if (quiet_flag) {
        return 0;
    }

    if (!f) {
        f = stderr;
    }

    va_start(args, fmt);
    ret = vfprintf(f, fmt, args);
    va_end(args);

    return ret;
}
