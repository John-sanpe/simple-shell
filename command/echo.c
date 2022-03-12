/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <command.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/uio.h>

static void error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputc('\n', stderr);
    exit(1);
}

static int echo_main(int argc, char *argv[])
{
    int nflag;
    int veclen;
    struct iovec *iov, *vp;
    char space[] = " ";
    char newline[] = "\n";
    char *progname = argv[0];

    if (*++argv && !strcmp(*argv, "-n")) {
        ++argv;
        --argc;
        nflag = 1;
    } else
        nflag = 0;

    veclen = (argc >= 2) ? (argc - 2) * 2 + 1 : 0;

    if ((vp = iov = malloc((veclen + 1) * sizeof(struct iovec))) == NULL)
        error("%s: write error", progname);

    while (argv[0] != NULL) {
        size_t len;

        len = strlen(argv[0]);
        if (argv[1] == NULL) {
            if (len >= 2 &&
                argv[0][len - 2] == '\\' &&
                argv[0][len - 1] == 'c') {
                len -= 2;
                nflag = 1;
            }
        }

        vp->iov_base = *argv;
        vp++->iov_len = len;

        if (*++argv) {
            vp->iov_base = space;
            vp++->iov_len = 1;
        }
    }

    if (!nflag) {
        veclen++;
        vp->iov_base = newline;
        vp++->iov_len = 1;
    }

    if (writev(STDOUT_FILENO, iov, veclen) == -1)
            error("%s: write error", progname);

    return 0;
}

struct sshell_command echo_command = {
    .name = "echo",
    .desc = "echo the string(s) to standard output",
    .exec = echo_main,
};
