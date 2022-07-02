/* DESCRIPTION: E93 OS Kernel Util Functions Source File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

void int_to_ascii(int n, char str[]) {
    int i = 0;
    int sign = n;

    if (sign < 0) {
        n = -n;
    }

    do {
        str[i++] = n % 10 + '0';
    } while((n /= 10) > 0);

    if (sign < 0) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // TODO: Implement reversing a string!!!
}

void memory_copy(char * source, char * destination, int n_bytes) {
    int i = 0;
    for (i = 0; i < n_bytes; i++) {
        *(destination + i) = *(source + i);
    }
}
