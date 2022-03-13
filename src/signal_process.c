#include <signal.h>


__sighandler_t sshell_singal_add(int sig, __sighandler_t handler) {
    return signal(sig, handler);
}
