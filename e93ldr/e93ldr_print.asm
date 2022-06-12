; DESCRIPTION: E93 OS Loader Printing Module Assembly File
; AUTHOR: Erdem Ersoy (eersoy93)
; COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
; LICENSE: MIT license. See README.md file for deatils.

print:
    pusha

print_loop:
    mov al, [bx]
    cmp al, 0
    je print_done

    ; Print shift out character for printing
    mov ah, 0x0e
    int 0x10

    add bx, 1
    jmp print_loop

    ; Print newline characters (LF + CR)
    mov al, 0x0a
    int 0x10
    mov al, 0x0d
    int 0x10

print_done:
    popa
    ret
