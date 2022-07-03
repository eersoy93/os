/* DESCRIPTION: E93 OS Interrupt Descriptor Table Header File
 * AUTHOR: Erdem Ersoy (eersoy93)
 * COPYRIGHT: Copyright (c) 2022 Erdem Ersoy (eersoy93).
 * LICENSE: MIT license. See README.md file for deatils.
 */

#ifndef IDT_H
#define IDT_H

#include "types.h"

// Segment selectors
#define KERNEL_CS (u16)0x08

// Interrupt gate handler struct
typedef struct {
    u16 offset_handler_function_address_low;
    u16 kernel_segment_selector;
    u8 always_zero;

    /* 7:   Interrupt is present
     * 6-5: CPU privilege level of the caller
     * 4:   Zero for interrupt gates
     * 3-0: 32-bit interrupt gate */
    u8 flags;
    u16 offset_handler_function_address_high;
} __attribute__((packed)) idt_gate_type;

// Interrupt handlers array pointer
typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idt_register_type;

#define IDT_ENTRIES_COUNT 256
idt_gate_type idt[IDT_ENTRIES_COUNT];
idt_register_type idt_register;

// IDT Functions
void set_idt_gate(int n, u32 handler);
void set_idt();

#endif
