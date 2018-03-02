#include <arch/i386/multiboot.h>
#include <drivers/vga/textmode.h>
#include <drivers/timer/timer.h>
#include <drivers/ps2/keyboard.h>
#include <stdio.h>
#include <arch/i386/gdt/gdt.h>
#include <arch/i386/int/idt.h>
#include <arch/i386/int/irq.h>

static int loop = 1;

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
void kernel_main(multiboot_info_t *mbi, uint32_t mb_magic) {
    gdt_init();
    irq_init();
    idt_init();

    textmode_init(textmode_make_attrib(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));

    textmode_write_string("Hello, terminal!\n");

    if (mbi->flags & (1 << 6)) {
        multiboot_memory_map_t *mmap;

        printf ("mmap_addr = 0x%x, mmap_length = 0x%x\n", (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);

        for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
                (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
                mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {

            printf (" size = 0x%x, base_addr = 0x%x%08x, length = 0x%x%08x, type = 0x%x\n",
                    (unsigned int) mmap->size,
                    (unsigned int) (mmap->addr >> 32),
                    (unsigned int) (mmap->addr & 0xffffffff),
                    (unsigned int) (mmap->len >> 32),
                    (unsigned int) (mmap->len & 0xffffffff),
                    (unsigned int) mmap->type);
        }
    }

    timer_install();
    keyboard_install();

    asm volatile ("sti");


    for (;;);
}
