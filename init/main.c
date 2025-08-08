#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "init.h"
#include "gfx.h"
#include "input.h"

int main(void)
{
    SYSTEM_STATE state = InitTheOS();

    // Prepare layout and terminal
    LAYOUT layout = ComputeLayout(&state);
    struct termios oldt;
    DisableTerminalEcho(&oldt);

    // Main loop
    while (1)
    {
        int key = ReadKey();
        if (key == 27) // ESC
            break;

        DrawInitialScreen(&state, &layout);
        sleep(1); // avoid busy waiting
    }

    // Restore terminal and cleanup
    RestoreTerminalEcho(&oldt);
    CleanupTheSytem(&state);
    PowerOffTheInit();

    return EXIT_SUCCESS;
}
