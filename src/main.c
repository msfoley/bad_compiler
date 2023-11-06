#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <args.h>
#include <log.h>
#include <lexer.h>

ssize_t read_file(const char *fname, char **buffer) {
    FILE *f = NULL;
    size_t len = 0;
    char *buf = NULL;

    f = fopen(fname, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        len = ftell(f);
        fseek(f, 0, SEEK_SET);

        buf = malloc(sizeof(*buf) * len);
        if (!buf) {
            return -ENOMEM;
        }

        fread(buf, 1, len, f);
        fclose(f);
    }

    *buffer = buf;
    return len;
}

int main(int argc, char **argv) {
    int ret;
    struct arguments *args;
    char *config_buf;
    struct token_list token_list;

    ret = parse_arguments(&args, argc, argv);
    if (ret) {
        return ret;
    }

    log_init(args->log, args->verbose, args->quiet);

    print_debug("Arguments:\n");
    print_debug("\tsource = %s\n", args->source);
    print_debug("\toutput = %s\n", args->output);
    print_debug("\tlog = %s\n", args->log);
    print_debug("\tverbose = %d\n", args->verbose);
    print_debug("\tquiet = %d\n", args->quiet);

    ssize_t fret = read_file(args->source, &config_buf);
    if (fret >= 0) {
        token_list.name = args->source;

        ret = parse_tokens(config_buf, &token_list);
        if (ret) {
            print_error("Tokenizing failed with %s (%d)\n", strerror(ret), ret);
        }

        free_token_list(&token_list);
    } else {
        print_error("Opening file failed with %s (%d)\n", strerror((int) -fret), (int) -fret);
    }

    log_exit();

    return 0;
}
