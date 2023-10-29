AC_INIT([binary_tool], [0.0], [mfoley@mfoley.xyz])
AM_INIT_AUTOMAKE([subdir-objects])

AC_PROG_CC
AC_CONFIG_FILES([Makefile])

AX_CHECK_COMPILE_FLAG([--std=c11], [
    CFLAGS += " --std=c11"
], [
    echo "C compiler cannot compile C11 code"
    exit -1
])

AC_OUTPUT
