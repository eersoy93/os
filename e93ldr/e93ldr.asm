; DESCRIPTION: E93 OS Loader Assembly File
; AUTHOR: Erdem Ersoy (eersoy93)
; COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
; LICENSE: MIT license. See README.md file for deatils.

; Specify global offset of the boot sector
[org 0x7c00]

; Print welcome message data and newline
mov bx, E93WELCOME
call print

; Infinite loop
jmp $

%include "e93ldr_print.asm"  ; E93LDR Printing Module

; Welcome message data
E93WELCOME:
    db 'Welcome to E93 Loader!', 0

; Boot sector padding and magic number BIOS checks
times 510-($-$$) db 0
dw 0xaa55
