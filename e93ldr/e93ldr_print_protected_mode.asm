; DESCRIPTION: E93 OS Loader Printing (Protected Module) Module Assembly File
; AUTHOR: Erdem Ersoy (eersoy93)
; COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
; LICENSE: MIT license. See README.md file for deatils.

[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_protected_mode:
    pusha
    mov edx, VIDEO_MEMORY

print_protected_mode_loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK

    cmp al, 0
    je print_protected_mode_done

    mov [edx], ax
    add ebx, 1
    add edx, 2

    jmp print_protected_mode_loop

print_protected_mode_done:
    popa
    ret
