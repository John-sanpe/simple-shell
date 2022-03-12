/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <command.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

static inline void usage(void)
{
    fprintf(stderr, "usage: pwd [OPTION]...\n");
    fprintf(stderr, "  -L  use PWD from environment\n");
    fprintf(stderr, "  -P  avoid all symlinks\n");
    exit(1);
}

static int pwd_main(int argc, char *argv[])
{
    char arg, phy = 0;
    char *p;

    while ((arg = getopt(argc, argv, "LP")) != -1)
        switch(arg) {
            case 'L':
                phy = 0;
                break;
            case 'P':
                phy = 1;
                break;
            default:
                usage();
        }

    argc -= optind;
    argv += optind;

    if (argc != 0)
        usage();

    if((phy && (p = getcwd(NULL, 0)) != NULL) ||
       ((p = getenv("PWD")) != NULL))
        printf("%s\n", p);
    else
        err(1, ".");

    return 0;
}

struct sshell_command pwd_command = {
    .name = "pwd",
    .desc = "print the full filename of the current working directory",
    .exec = pwd_main,
};
