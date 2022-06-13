/* DESCRIPTION: E93 OS Port Drivers Header File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);

unsigned short port_word_in(unsigned short port);
void port_word_out(unsigned short port, unsigned short data);
