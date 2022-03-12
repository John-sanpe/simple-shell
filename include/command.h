/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <sshell.h>

extern struct sshell_command echo_command;
extern struct sshell_command hostname_command;
extern struct sshell_command pwd_command;
extern struct sshell_command rmdir_command;
extern struct sshell_command sleep_command;
extern struct sshell_command sync_command;

#endif  /* _COMMAND_H_ */
