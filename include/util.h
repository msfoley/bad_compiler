#ifndef UTIL_H
#define UTIL_H

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

int test_read(const char *file);
int test_write(const char *file);

#endif
