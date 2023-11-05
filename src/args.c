#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <argp.h>

#include <config.h>
#include <args.h>
#include <util.h>
#include <log.h>

static struct argp_option options[] = {
    { "quiet", 'q', 0, 0, "Disable output" },
    { "silent", 's', 0, OPTION_ALIAS },
    { "verbose", 'v', 0, OPTION_ARG_OPTIONAL, "Enable verbose options" },
    { "output", 'o', "FILE", 0, "Output to FILE instead of modifying source" },
    { "log", 'l', "FILE", 0, "Log to FILE instead of stderr/stdout" },
    { 0 }
};

const char *argp_program_bug_address = PROGRAM_EMAIL;
const char *argp_program_version = PROGRAM_NAME " v" PROGRAM_VERSION;

const char doc[] = PROGRAM_NAME " -- Bad C compiler";
const char args_doc[] = "SOURCE_FILE";

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = (struct arguments *) state->input;

    switch (key) {
        case 'q':
        case 's':
            arguments->quiet = 1;
            break;
        case 'v':
            arguments->verbose = 1;
            break;
        case 'o':
            arguments->output = arg;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 1) {
                argp_usage(state);
            } else if (state->arg_num == 0) {
                arguments->source = arg;
            }

            break;
        case ARGP_KEY_END:
            if (state->arg_num < 1) {
                argp_usage(state);
            }

            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

const struct argp argp = { options, parse_opt, args_doc, doc };

int parse_arguments(struct arguments **arguments, int argc, char **argv) {
    struct arguments *args;
    int ret;

    *arguments = NULL;

    if (!arguments) {
        return EINVAL;
    }

    args = calloc(1, sizeof(*args));
    if (!args) {
        return ENOMEM;
    }

    ret = argp_parse(&argp, argc, argv, 0, 0, args);
    if (ret) {
        print_error("Error parsing arguments\n");

        goto cleanup;
    }

    if (args->verbose && args->quiet) {
        print_error("Verbose and quiet cannpt specified at the same time\n");

        ret = EINVAL;
        goto cleanup;
    }

    if (args->log) {
        ret = test_write(args->log);
        if (ret) {
            goto cleanup;
        }
    }

    ret = test_read(args->source);
    if (ret) {
        goto cleanup;
    }

    if (!args->output) {
        args->output = "a.out";
    }

    if (args->output) {
        ret = test_write(args->output);
        if (ret) {
            goto cleanup;
        }
    }

    *arguments = args;

    return ret;
cleanup:
    free(args);

    return ret;
}
