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

static int pflag;
static int vflag;

static inline void usage(void)
{
    fprintf(stderr, "usage: rmdir [-pv] directory ...\n");
    exit(1);
}

static int rm_dir(const char *path)
{
    char *ch;

    if (rmdir(path)) {
        warn("%s", path);
        return 1;
    }

    if (vflag)
        printf("%s\n", path);

    if (pflag) {
        ch = (char *)(path + strlen(path));
        if (*--ch == '/')
            *ch = '\0';
        ch = strrchr(path, '/');
        if (path < ch) {
            *ch = '\0';
            return rm_dir(path);
        }
    }

    return 0;
}

static int rmdir_main(int argc, char *argv[])
{
    char arg, ret;

    while ((arg = getopt(argc, argv, "pv")) != -1)
        switch (arg) {
            case 'p':
                pflag = 1;
                break;
            case 'v':
                vflag = 1;
                break;
            default:
                usage();
        }

    argc -= optind;
    argv += optind;

    if (argc == 0)
        usage();

    while (*argv)
        ret |= rm_dir(*argv++);

    exit(ret);
}

struct sshell_command rmdir_command = {
    .name = "rmdir",
    .desc = "remove the directory",
    .exec = rmdir_main,
};
