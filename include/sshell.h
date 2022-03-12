/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2021 Sanpe <sanpeqf@gmail.com>
 */

#ifndef _SSHELL_H_
#define _SSHELL_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <errno.h>
#include <list.h>

typedef unsigned int (*readline_read_t)(char *str, unsigned int len, void *data);
typedef void (*readline_write_t)(const char *str, unsigned int len, void *data);

enum readline_esc {
    READLINE_ESC_NORM   = 0,
    READLINE_ESC_ESC    = 1,
    READLINE_ESC_CSI    = 2,
    READLINE_ESC_SS3    = 3,
};

struct readline_history {
    struct list_head list;
    unsigned int len;
    char cmd[];
};

struct readline_state {
    readline_read_t read;
    readline_write_t write;
    void *data;

    char *buff;
    unsigned int bsize;
    const char *prompt;
    unsigned int plen;
    unsigned int len;
    unsigned int pos;
    char esc_param;
    enum readline_esc esc_state;
    struct list_head history;
    struct readline_history *curr;
};

struct sshell_command {
    const char *name;
    const char *desc;
    struct list_head list;
    state (*exec)(int argc, char *argv[]);
};

#define list_to_kshell(ptr) \
    container_of(ptr, struct sshell_command, list)

extern unsigned int sshell_read(char *str, unsigned int len, void *data);
extern void sshell_write(const char *str, unsigned int len, void *data);

extern char *readline(struct readline_state *state, const char *prompt);
extern struct readline_state *readline_alloc(readline_read_t read, readline_write_t write, void *data);
extern void readline_free(struct readline_state *state);

extern state sshell_parser(char *cmdline, char **pos, int *argc, char ***argv);
extern state sshell_main(int argc, char *argv[]);
extern struct sshell_command *kshell_find(const char *name);

extern state sshell_system(const char *cmdline);
extern state sshell_register(struct sshell_command *);
extern void sshell_unregister(struct sshell_command *);
extern void sshell_init(void);

#endif  /* _KSHELL_PRIV_H_ */
