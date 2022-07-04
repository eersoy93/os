/* DESCRIPTION: E93 OS Kernel Util Functions Header File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

#ifndef UTILS_H
#define UTILS_H

#include "../cpu/types.h"

void int_to_ascii(int n, char str[]);
void memory_copy(char * source, char * destination, int n_bytes);
void memory_set(u8 * destination, u8 value, u32 length);

#endif
