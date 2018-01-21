#include "idt.h"

#include <string.h>

struct idt_pointer {
    uint16_t limit;
    uintptr_t base;
} __attribute__((packed));

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute((packed));

static struct idt_pointer idt_ptr;
static struct idt_entry idt_entries[256];

extern void idt_flush(uintptr_t);
extern void isr_register_stubs(void);

static void idt_set_gate(uint8_t index, uint32_t base, uint16_t selector, uint8_t flags) {
    idt_entries[index].base_low = base & 0xFFFF;
    idt_entries[index].base_high = (base >> 16) & 0xFFFF;

    idt_entries[index].selector = selector;
    idt_entries[index].always0 = 0;

    //TODO: Uncomment when we 0x60 when we get to user mode.
    idt_entries[index].flags = flags /* | 0x60 */;
}

void idt_register_isr(uint8_t index, uint32_t base) {
    idt_set_gate(index, base, 0x08, 0x8E);
}

void idt_init(void) {
    idt_ptr.limit = sizeof(idt_entries) - 1;
    idt_ptr.base = (uintptr_t) &idt_entries;
    
    memset(&idt_entries, 0, sizeof(idt_entries));

    isr_register_stubs();
    idt_flush((uintptr_t) &idt_ptr);
}
