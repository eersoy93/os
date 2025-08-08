#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/reboot.h>

int main(void)
{
    // Request shutdown using reboot() system call
    sync();
    reboot(RB_POWER_OFF);

    // If running as init (PID 1), never exit: stay alive to avoid kernel panic
    if (getpid() == 1)
    {
        while (1) pause();
    }

    return EXIT_SUCCESS;
}
