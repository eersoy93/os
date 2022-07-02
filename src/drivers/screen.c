/* DESCRIPTION: E93 OS Screen Driver Source File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

#include "ports.h"
#include "screen.h"
#include "../kernel/utils.h"


// Private Kernel API Declarations

int get_cursor_offset();
int get_offset(int col, int row);
int get_offset_col(int offset);
int get_offset_row(int offset);
int printk_char(char character, int col, int row, char attribute);
void set_cursor_offset(int offset);


// Public Kernel API Definitions

// Clear entire screen with black
void clear_screen() {
    int screen_size = COLS_MAX * ROWS_MAX;
    int i = 0;
    char * screen = VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = WHITE_ON_BLACK;
    }

    set_cursor_offset(get_offset(0, 0));
}

// Print message at cursor location
void printk(char * message) {
    printk_at(message, -1, -1);
}

// Print message at specific location
void printk_at(char * message, int col, int row) {
    int offset = 0;

    if (col >= 0 && row >= 0) {
        offset = get_offset(col, row);
    }
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int i = 0;

    while (message[i] != 0) {
        offset = printk_char(message[i++], col, row, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}


// Private Kernel API Definitions

// Get cursor video memory offset
int get_cursor_offset() {
    port_byte_out(PORT_SCREEN_CTRL, 14);
    int offset = port_byte_in(PORT_SCREEN_DATA) << 8;
    port_byte_out(PORT_SCREEN_CTRL, 15);
    offset += port_byte_in(PORT_SCREEN_DATA);

    return offset * 2;
}

// Get specific location video memory offset
int get_offset(int col, int row) {
    return 2 * (row * COLS_MAX + col);
}

// Get specific column of video memory offset
int get_offset_col(int offset) {
    return (offset - (get_offset_row(offset) * 2 * COLS_MAX)) / 2;
}

// Get specific row of video memory offset
int get_offset_row(int offset) {
    return offset / (2 * COLS_MAX);
}

// Print specific character with specific attribute at specific loaction
int printk_char(char character, int col, int row, char attribute) {
    unsigned char * video_memory = (unsigned char *) VIDEO_ADDRESS;
    if (!attribute) {
        attribute = WHITE_ON_BLACK;
    }

    if (col >= COLS_MAX || row >= ROWS_MAX) {
        video_memory[2 * COLS_MAX * ROWS_MAX - 2] = 'E';
        video_memory[2 * COLS_MAX * ROWS_MAX - 1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset = 0;
    if (col >= 0 && row >= 0) {
        offset = get_offset(col, row);
    }
    else {
        offset = get_cursor_offset();
    }

    if (character == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1) ;
    }
    else {
        video_memory[offset] = character;
        video_memory[offset + 1] = attribute;
        offset += 2;
    }

    if (offset >= ROWS_MAX * COLS_MAX * 2) {
        int i = 0;
        for (i = 1; i < ROWS_MAX; i++) {
            memory_copy(get_offset(0, i) + VIDEO_ADDRESS,
                        get_offset(0, i - 1) + VIDEO_ADDRESS,
                        COLS_MAX * 2);
        }

    char * lastline = get_offset(0, ROWS_MAX - 1) + VIDEO_ADDRESS;
    for (i = 0; i < COLS_MAX * 2; i++) {
        lastline[i] = 0;
    }

    offset -= COLS_MAX * 2;
    }

    set_cursor_offset(offset);
    return offset;
}

// Set cursor video memory offset
void set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(PORT_SCREEN_CTRL, 14);
    port_byte_out(PORT_SCREEN_DATA, (unsigned char) (offset >> 8));
    port_byte_out(PORT_SCREEN_CTRL, 15);
    port_byte_out(PORT_SCREEN_DATA, (unsigned char) (offset & 0xff));
}
