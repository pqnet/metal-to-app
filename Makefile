
CC=clang-9
ISOFILE=windows/img.iso

TARGET=x86_64-none-elf
FLAGS=-Werror -Wall -g -O2 -nostdlib -static -target $(TARGET)
CFLAGS=-std=gnu2x -mcmodel=kernel -mno-red-zone -mno-sse
LINK_FLAGS=-fno-exceptions -fno-unwind-tables -ffreestanding -Wl,-z,max-page-size=0x200000 -Wl,-n -Wl,--build-id=none

$(ISOFILE): kernel testelf iso/boot/grub/grub.cfg
	cp kernel iso
	cp testelf iso
	grub-mkrescue -d /usr/lib/grub/i386-pc -o $@ iso

OBJECTS=\
src/memset.o src/memcpy.o src/multiboot.o\
src/bootstrap.o src/interrupts.o src/print.o\
src/exceptions.o src/keyboard.o src/scancodes.o\
src/scheduler.o src/scheduler_asm.o src/test_scheduler.o\
src/frame.o src/memory.o src/address_space.o\
src/main.o

kernel: linker.ld $(OBJECTS)
	$(CC) $(FLAGS) $(LINK_FLAGS) -Wl,-T,$^ -o $@
	grub-file --is-x86-multiboot $@

%.o: %.s
	$(CC) $(FLAGS) $^ -c -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(FLAGS) $^ -c -o $@

#src/interrupts.o: src/interrupts.c
#	$(CC) $(CFLAGS) $(FLAGS) -mcmodel=large $^ -c -o $@

.PHONY: clean

clean:
	rm -f windows/img.iso
	rm -f kernel
	rm -f testelf
	rm -f src/*.o


	
testelf: modules/testelf/src/testelf.o
	$(CC) $(FLAGS) $(LINK_FLAGS) $^ -o $@
