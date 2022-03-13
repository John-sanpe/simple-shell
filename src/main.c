/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#include <command.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "signal_process.h"
struct termios orgin;
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
static void 
sshell_set_orgin_termios_sig_handler(int sig) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orgin);
    psignal(sig, NULL);
    exit(-1);
}
static void 
sshell_set_signal() {
    sshell_singal_add(SIGSEGV, sshell_set_orgin_termios_sig_handler);
}
int main(int argc, char *argv[])
{
    struct termios info;
    tcgetattr(STDIN_FILENO, &orgin);
    tcgetattr(STDIN_FILENO, &info);

    info.c_lflag &= ~(ICANON | ISIG | ECHO);
    info.c_iflag &= ~BRKINT;
    info.c_cc[VTIME] = 255;
    info.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &info);
    sshell_set_signal();

    builtin_init();
    sshell_init();

    tcsetattr(STDIN_FILENO, TCSANOW, &orgin);

    return 0;
}
