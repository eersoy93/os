/* DESCRIPTION: E93 OS Interrupt Descriptor Table Code File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

#include "idt.h"
#include "../kernel/utils.h"

void set_idt_gate(int n, u32 handler) {
    idt[n].offset_handler_function_address_low = low16(handler);
    idt[n].kernel_segment_selector = KERNEL_CS;
    idt[n].always_zero = (u8) 0;
    idt[n].flags = 0x8E; // 1000 1110
    idt[n].offset_handler_function_address_high = high16(handler);
}

void set_idt() {
    idt_register.base = (u32) &idt;
    idt_register.limit = sizeof(idt_gate_type) * IDT_ENTRIES_COUNT - 1;
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_register));
}
