/* DESCRIPTION: E93 OS Kernel Main Source File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

#include "../drivers/screen.h"

void main() {
    clear_screen();
    printk("Executing the kernel...\n");
    printk("Welcome to E93 OS!\n");
}
