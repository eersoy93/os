; DESCRIPTION: E93 OS Loader Assembly File
; AUTHOR: Erdem Ersoy (eersoy93)
; COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
; LICENSE: MIT license. See README.md file for deatils.

; Specify global offset of the boot sector
[org 0x7c00]

; Enter TTY mode
mov ah, 0x0e

; Write "\nE93LDR"
mov al, 10
int 0x10
mov al, 13
int 0x10
mov al, 'E'
int 0x10
mov al, '9'
int 0x10
mov al, '3'
int 0x10
mov al, 'L'
int 0x10
mov al, 'D'
int 0x10
mov al, 'R'
int 0x10

jmp $  ;Infinite loop

; Boot sector padding and magic number BIOS checks
times 510-($-$$) db 0
dw 0xaa55
