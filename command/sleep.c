/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <command.h>
#include <stdlib.h>
#include <unistd.h>

static int sleep_main(int argc, char *argv[])
{
    sleep(strtoul(argv[1], NULL, 0));
    return 0;
}

struct sshell_command sleep_command = {
    .name = "sleep",
    .desc = "pause for number seconds",
    .exec = sleep_main,
};
