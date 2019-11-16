
CC=clang-9
ISOFILE=windows/img.iso

TARGET=x86_64-none-elf
# note: we can't enable O2 because the size of relocation symbols for 
FLAGS=-g -O0 -nostdlib -static -target $(TARGET)
CFLAGS=-mcmodel=kernel -mno-red-zone -mno-sse
LINK_FLAGS=-fno-exceptions -fno-unwind-tables -ffreestanding -z max-page-size=0x200000 -Wl,-n -Wl,--build-id=none

$(ISOFILE): kernel iso/boot/grub/grub.cfg
	cp kernel iso
	grub-mkrescue -d /usr/lib/grub/i386-pc -o $@ iso

OBJECTS=\
src/bootstrap.o src/interrupts.o src/print.o\
src/exceptions.o src/keyboard.o src/scancodes.o\
src/scheduler.o src/scheduler_asm.o src/test_scheduler.o\
src/frame.o\
src/main.o

kernel: linker.ld $(OBJECTS)
	$(CC) $(FLAGS) $(LINK_FLAGS) -Wl,-T,$^ -o $@
	grub-file --is-x86-multiboot $@

%.o: %.s
	$(CC) $(FLAGS) $^ -c -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(FLAGS) $^ -c -o $@

src/interrupts.o: src/interrupts.c
	$(CC) $(CFLAGS) $(FLAGS) -mcmodel=large $^ -c -o $@

.PHONY: clean

clean:
	rm -f windows/img.iso
	rm -f kernel
	rm -f src/*.o