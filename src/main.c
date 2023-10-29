#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <args.h>
#include <log.h>

int main(int argc, char **argv) {
    int ret;
    struct arguments *args;

    ret = parse_arguments(&args, argc, argv);
    if (ret) {
        return ret;
    }

    log_init(args->log, args->verbose, args->quiet);

    print_info("config = %s\n", args->config);
    print_info("source = %s\n", args->source);
    print_info("output = %s\n", args->output);
    print_info("log = %s\n", args->log);
    print_info("verbose = %d\n", args->verbose);
    print_info("quiet = %d\n", args->quiet);

    log_exit();

    return 0;
}
