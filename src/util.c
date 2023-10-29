#include <stdio.h>
#include <errno.h>

#include <util.h>
#include <log.h>

int test_read(const char *file) {
    FILE *f;

    f = fopen(file, "r");
    if (f == NULL) {
        print_error("Could not open %s for reading\n", file);

        return EIO;
    }

    fclose(f);
    return 0;
}

int test_write(const char *file) {
    FILE *f;

    f = fopen(file, "w");
    if (f == NULL) {
        print_error("Could not open %s for writing.\n", file);

        return EIO;
    }

    fclose(f);
    return 0;
}
