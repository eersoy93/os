; DESCRIPTION: E93 OS Loader Assembly File
; AUTHOR: Erdem Ersoy (eersoy93)
; COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
; LICENSE: MIT license. See README.md file for deatils.

; Specify global offset of the boot sector
[org 0x7c00]

mov bp, 0x9000
mov sp, bp

mov bx, MSG_WELCOME
call print

mov bx, MSG_ENTER_REALMODE
call print

call switch_to_protected_mode
jmp $

%include "e93ldr_print.asm"  ; E93LDR Printing Module
%include "e93ldr_gdt.asm"  ; E93LDR Global Descriptor Table Module
%include "e93ldr_switch_to_protected_mode.asm"  ; E93LDR Switch to Protected Mode Module
%include "e93ldr_print_protected_mode.asm"  ; E93LDR Printing Module (for Protected Mode)

[bits 32]

begin_protected_mode:
    mov ebx, MSG_ENTER_PROTECTEDMODE
    call print_protected_mode
    jmp $  ; infinite loop

; Messages data
MSG_WELCOME db "Welcome to E93 Loader!", 0
MSG_ENTER_REALMODE db "Started in 16-bit real mode!", 0
MSG_ENTER_PROTECTEDMODE db "Started in 32-bit protected mode!", 0

; Boot sector padding and magic number BIOS checks
times 510-($-$$) db 0
dw 0xaa55
