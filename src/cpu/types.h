/* DESCRIPTION: E93 OS Machine-Dependent (x86-32) Header File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

#ifndef TYPES_H
#define TYPES_H

typedef unsigned int u32;
typedef int i32;
typedef unsigned short u16;
typedef short i16;
typedef unsigned char u8;
typedef char i8;

#define low16(address) (u16)((address) & 0xFFFF)
#define high16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif
