/* DESCRIPTION: E93 OS Port Drivers Source File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

// Read byte from port
unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al": "=a" (result) : "d" (port));
    return result;
}

// Write byte to port
void port_byte_out(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx": : "a" (data), "d" (port));
}

// Read word from port
unsigned char port_word_in(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%ax": "=a" (result) : "d" (port));
    return result;
}

// Write word to port
void port_word_out(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx": : "a" (data), "d" (port));
}
