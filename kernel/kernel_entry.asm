; DESCRIPTION: E93 OS Kernel Entry Point Assembly File
; AUTHOR: Erdem Ersoy (eersoy93)
; COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
; LICENSE: MIT license. See README.md file for deatils.

[bits 32]

[extern main]
call main
jmp $
