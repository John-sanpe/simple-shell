/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <command.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

static struct sshell_command *builtin_command[] = {
    &echo_command,
    &hostname_command,
    &pwd_command,
    &rmdir_command,
    &sleep_command,
    &sync_command,
};

static state builtin_init(void)
{
    unsigned int count;
    state retval;

    for (count = 0; count < ARRAY_SIZE(builtin_command); ++count) {
        retval = sshell_register(builtin_command[count]);
        if (unlikely(retval))
            return retval;
    }

    return -ENOERR;
}

unsigned int sshell_read(char *str, unsigned int len, void *data)
{
    *str = getchar();
    return 1;
}

void sshell_write(const char *str, unsigned int len, void *data)
{
    printf(str);
}

int main(int argc, char *argv[])
{
    struct termios info;

    tcgetattr(0,&info);
    info.c_lflag &= ~ECHO;
    tcsetattr(0,TCSANOW,&info);

    builtin_init();
    sshell_init();

    return 0;
}
