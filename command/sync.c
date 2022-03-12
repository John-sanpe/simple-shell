/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <command.h>
#include <unistd.h>

static int sync_main(int argc, char *argv[])
{
    sync();
    return 0;
}

struct sshell_command sync_command = {
    .name = "sync",
    .desc = "synchronize cached writes to persistent storage",
    .exec = sync_main,
};
