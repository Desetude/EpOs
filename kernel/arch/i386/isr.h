#pragma once

#include <stdint.h>

struct int_state {
    uint32_t ds; //Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; //Registers
    uint32_t int_no, err_code; //Interrupt number and error code (if pushed)
    uint32_t eip, cs, eflags, useresp, ss; //Pushed by CPU
};

void isr_handler(struct int_state state);
