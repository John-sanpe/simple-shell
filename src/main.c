/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <command.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
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
    return read(STDIN_FILENO, str, len);
}

void sshell_write(const char *str, unsigned int len, void *data)
{
    write(STDOUT_FILENO, str, len);
}

int main(int argc, char *argv[])
{
    struct termios info, save;

    tcgetattr(STDIN_FILENO, &info);
    save = info;
    info.c_lflag &= ~(ICANON | ISIG | ECHO);
    info.c_iflag &= ~BRKINT;
    info.c_cc[VTIME] = 255;
    info.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &info);

    builtin_init();
    sshell_init();

    tcsetattr(STDIN_FILENO, TCSANOW, &save);

    return 0;
}
