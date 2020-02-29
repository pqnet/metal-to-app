
CC=clang-9
ISOFILE=windows/img.iso

TARGET=x86_64-none-elf
FLAGS=-Werror -Wall -g -O2 -nostdlib -static -target $(TARGET)
CFLAGS=-std=gnu2x -mcmodel=kernel -mno-red-zone -mno-sse

# -Wl,-z,max-page-size=0x200000
LINK_FLAGS=-fno-exceptions -fno-unwind-tables -ffreestanding -Wl,-n -Wl,--build-id=none

$(ISOFILE): kernel testelf iso/boot/grub/grub.cfg
	cp kernel iso
	cp testelf iso
	grub-mkrescue -d /usr/lib/grub/i386-pc -o $@ iso

OBJECTS=\
src/memset.o src/memcpy.o src/multiboot.o\
src/initrd.o src/elf.o src/userspace.o\
src/bootstrap.o src/interrupts.o src/print.o\
src/exceptions.o src/keyboard.o src/scancodes.o\
src/scheduler.o src/scheduler_asm.o src/test_scheduler.o\
src/frame.o src/memory.o src/address_space.o\
src/tss.o\
src/syscall_asm.o src/syscall.o src/syscall_table.o\
src/main.o

kernel: linker.ld $(OBJECTS) testelf
	$(CC) $(FLAGS) $(LINK_FLAGS) -Wl,-T,linker.ld $(OBJECTS) -o $@
	objcopy --add-gnu-debuglink=testelf kernel
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
	rm -f modules/*/src/*.o


	
testelf: modules/testelf/src/testelf.o modules/vdso/src/vdso.o
	$(CC) $(FLAGS) $(LINK_FLAGS) $^ -o $@
