/* DESCRIPTION: E93 OS Screen Driver Header File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

#ifndef SCREEN_H
#define SCREEN_H


// Video Constants Definitions

#define VIDEO_ADDRESS (char *) 0xb8000
#define ROWS_MAX 25
#define COLS_MAX 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

// Screen I/O Ports Definitions

#define PORT_SCREEN_CTRL 0x3d4
#define PORT_SCREEN_DATA 0x3d5

// Public Kernel API Declarations

void clear_screen();
void kprint(char * message);
void kprint_at(char * message, int col, int row);

#endif
