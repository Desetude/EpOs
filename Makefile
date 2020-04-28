KERNEL := kernel/hafos
SYSROOT := sysroot
ISO := hafos.iso

TARGET := i686-elf

CC := $(TARGET)-gcc

WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wredundant-decls -Wno-unused-parameter \
            -Wnested-externs -Winline -Wno-long-long -Wstrict-prototypes

CFLAGS := -std=gnu11 -ffreestanding -nostdlib -lgcc \
	  $(WARNINGS)

QEMU_FLAGS := -no-shutdown -no-reboot -d unimp,guest_errors,cpu_reset

SRC_FILES := $(shell find kernel -type f -name "*.c" -or -name "*.s")
OBJ_FILES := $(addsuffix .o, $(basename $(SRC_FILES)))
DEP_FILES := $(addsuffix .d, $(basename $(SRC_FILES)))

all: $(ISO)

$(KERNEL): $(OBJ_FILES) kernel/linker.ld
	$(CC) $(CFLAGS) -T kernel/linker.ld -o $@ $(OBJ_FILES)

$(ISO): $(KERNEL) $(SYSROOT)/boot/grub/grub.cfg
	cp $(KERNEL) $(SYSROOT)/boot
	grub-mkrescue -o $(ISO) $(SYSROOT)

run: $(KERNEL)
	qemu-system-i386 $(QEMU_FLAGS) -kernel $(KERNEL)

debug: CFLAGS += -ggdb
debug: QEMU_FLAGS += -s -S
debug: $(KERNEL)
	qemu-system-i386 $(QEMU_FLAGS) -kernel $(KERNEL)

debug_gdb:
	gdb $(KERNEL) -ex 'set architecture i386' -ex 'target remote localhost:1234'

clean:
	rm -f $(OBJ_FILES) $(DEP_FILES) $(KERNEL) $(ISO)

%.o: %.c
	$(CC) $(CFLAGS) -MD -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -MD -c $< -o $@
