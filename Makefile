# SPDX-License-Identifier: GPL-2.0-or-later
CFLAGS := -g -Wall -Werror -I include/

head := sshell.h list.h macro.h
head := $(addprefix include/, $(head))

objs := src/main.o
objs += src/sshell.o
objs += src/parser.o
objs += src/readline.o
objs += command/echo.o
objs += command/hostname.o
objs += command/pwd.o
objs += command/rmdir.o
objs += command/sleep.o
objs += command/sync.o

%.o:%.c $(head)
	@ echo -e "  \e[32mCC\e[0m	" $@
	@ gcc -o $@ -c $< $(CFLAGS)

simple-shell: $(objs) $(head)
	@ echo -e "  \e[34mMKELF\e[0m	" $@
	@ gcc -o $@ $(objs) $(CFLAGS)

clean:
	@ rm -f $(objs) simple-shell
