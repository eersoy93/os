/* DESCRIPTION: E93 OS Kernel Main Source File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

#include "../drivers/screen.h"
#include "utils.h"

void main() {
    clear_screen();

    int i = 0;
    for(i = 0; i < 24; i++) {
        char str[255];
        int_to_ascii(i, str);
        printk_at(str, 0, i);
    }

    printk("\n");
    printk("Executing the kernel...\n");
    printk("Welcome to E93 OS!\n");

}
