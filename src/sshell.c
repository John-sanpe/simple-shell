/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <sshell.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

static LIST_HEAD(sshell_list);

struct sshell_command *sshell_find(const char *name)
{
    struct sshell_command *cmd, *find = NULL;

    list_for_each_entry(cmd, &sshell_list, list) {
        if (!strcmp(name, cmd->name)) {
            find = cmd;
            break;
        }
    }

    return find;
}

state sshell_register(struct sshell_command *cmd)
{
    if (!cmd->name || !cmd->desc)
        return -EINVAL;

    if (sshell_find(cmd->name)) {
        printf("cmd %s already registered", cmd->name);
        return -EINVAL;
    }

    list_add(&sshell_list, &cmd->list);
    return -ENOERR;
}

void sshell_unregister(struct sshell_command *cmd)
{
    list_del(&cmd->list);
}

static state do_system(char *cmdline, jmp_buf buff)
{
    struct sshell_command *cmd;
    char **argv;
    int argc;
    state ret;

    while (cmdline) {
        ret = sshell_parser(cmdline, &cmdline, &argc, &argv);
        if (ret)
            return ret;

        cmd = sshell_find(argv[0]);
        if (!cmd) {
            printf("command not found: %s\n", argv[0]);
            return -EBADF;
        }

        if (buff && !strcmp(argv[0], "exit"))
            longjmp(buff, true);

        if (!cmd->exec)
            return -ENXIO;

        ret = cmd->exec(argc, argv);
        if (ret)
            return ret;
    }

    return ret;
}

state sshell_system(const char *cmdline)
{
    char *new;
    state ret;

    new = strdup(cmdline);
    ret = do_system(new, NULL);
    free(new);

    return ret;
}

state sshell_main(int argc, char *argv[])
{
    struct readline_state *rstate;
    char *cmdline, retbuf[20];
    jmp_buf buff;
    state ret, exit;

    rstate = readline_alloc(sshell_read, sshell_write, NULL);
    if (!rstate)
        return -ENOMEM;

    exit = setjmp(buff);

    while (!exit) {
        cmdline = readline(rstate, "kshell: # ");
        if (!rstate->len)
            continue;
        ret = do_system(cmdline, buff);
        snprintf(retbuf, sizeof(retbuf), "%d", ret);
        setenv("?", retbuf, true);
    }

    readline_free(rstate);
    return ret;
}

state help_main(int argc, char *argv[])
{
    struct sshell_command *cmd;
    list_for_each_entry(cmd, &sshell_list, list)
        printf("%s\t- %s\n", cmd->name, cmd->desc);
    return -ENOERR;
}

static struct sshell_command sshell_cmd = {
    .name = "kshell",
    .desc = "kshell interpreter",
    .exec = sshell_main,
};

static struct sshell_command help_cmd = {
    .name = "help",
    .desc = "displays all available instructions",
    .exec = help_main,
};

static struct sshell_command exit_cmd = {
    .name = "exit",
    .desc = "exit the sshell interpreter",
};

void sshell_init(void)
{
    printf("#######################\n");
    printf("Welcome to simple shell\n");

    sshell_register(&sshell_cmd);
    sshell_register(&help_cmd);
    sshell_register(&exit_cmd);

    printf("Have a lot of fun..\n");
    sshell_main(0, NULL);
}
