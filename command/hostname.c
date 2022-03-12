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

#define HOSTNAME_MAX    1024
#define HOSTNAME_SHORT  1
#define HOSTNAME_DOMAIN 2
#define HOSTNAME_FILE   4

static inline void usage(void)
{
    fprintf(stderr, "usage: hostname [OPTION...] [NAME]\n");
    fprintf(stderr, "  -s  short host name\n");
    fprintf(stderr, "  -d  DNS domain name\n");
    fprintf(stderr, "  -f  DNS host name or FQDN\n");
    fprintf(stderr, "  -F  set host name or NIS domain name from FILE\n");
    exit(1);
}

static inline void set_hostname(char *name)
{
    if (sethostname(name, (int)strlen(name)))
        err(1, "sethostname");
}

static int hostname_main(int argc, char *argv[])
{
    int flags = 0;
    char *p, arg, hostname[HOSTNAME_MAX];

    while ((arg = getopt(argc, argv, "sdfF")) != -1)
        switch (arg) {
            case 's':
                flags |= HOSTNAME_SHORT;
                break;
            case 'd':
                flags |= HOSTNAME_DOMAIN;
                break;
            case 'F':
                flags |= HOSTNAME_FILE;
                break;
            case 'f':
                break;
            default:
                usage();
        }

    argc -= optind;
    argv += optind;

    if ((!flags || (!!(flags & HOSTNAME_SHORT) ^ !!(flags & HOSTNAME_DOMAIN))) && !argc) {
        if (gethostname(hostname, (int)sizeof(hostname)))
            err(1, "gethostname");
        p = strchr(hostname, '.');
        if (p) {
            if (flags & HOSTNAME_SHORT)
                *p = '\0';
            if (flags & HOSTNAME_DOMAIN)
                strcpy(hostname, ++p);
        }
        printf("%s\n", hostname);
    } else if (flags & HOSTNAME_FILE && argc == 1) {
        FILE *fp;
        fp = fopen(*argv, "r");
        if (!fp)
            err(1, "fopen");
        fscanf(fp, "%s", (char *)&hostname);
        fclose(fp);
        set_hostname((char *)&hostname);
    } else if (!flags && argc == 1) {
        set_hostname(*argv);
    } else {
        usage();
    }

    return 0;
}

struct sshell_command hostname_command = {
    .name = "hostname",
    .desc = "show or set the system's host name",
    .exec = hostname_main,
};
