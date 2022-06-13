/* DESCRIPTION: E93 OS Kernel Main Source File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

#include "../drivers/ports.h"

void main() {
    // Query cursor position high byte
    port_byte_out(0x3d4, 14);

    // Read cursor position high byte and place it
    int position = port_byte_in(0x3d5);
    position = position << 8;

    // Query cursor position low byte
    port_byte_out(0x3d4, 15);

    // Read cursor position low byte and place it
    position += port_byte_in(0x3d5);

    // Create VGA offset
    int offset_from_vga = position * 2;

    char * vga = 0xb8000; // 0xb8000 is video memory address.

    // Write cyan "!" on red background
    vga[offset_from_vga] = '!';
    vga[offset_from_vga + 1] = 0x43;
}
