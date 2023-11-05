#ifndef ARGS_H
#define ARGS_H

struct arguments {
    const char *source;
    const char *output;
    const char *log;
    int verbose;
    int quiet;
};

int parse_arguments(struct arguments **arguments, int argc, char **argv);

#endif
