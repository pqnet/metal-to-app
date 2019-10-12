
CC=clang-9
ISOFILE=windows/img.iso

TARGET=x86_64-none-elf
FLAGS=-g -O1 -nostdlib -static -target $(TARGET)
CFLAGS=-mcmodel=kernel -mno-red-zone -mno-sse
LINK_FLAGS=-fno-exceptions -fno-unwind-tables -ffreestanding -z max-page-size=0x200000 -Wl,-n -Wl,--build-id=none

$(ISOFILE): kernel iso/boot/grub/grub.cfg
	cp kernel iso
	grub-mkrescue -d /usr/lib/grub/i386-pc -o $@ iso

OBJECTS=src/bootstrap.o src/main.o src/interrupts.o src/print.o

kernel: linker.ld $(OBJECTS)
	$(CC) $(FLAGS) $(LINK_FLAGS) -Wl,-T,$^ -o $@
	grub-file --is-x86-multiboot $@

%.o: %.s
	$(CC) $(FLAGS) $^ -c -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(FLAGS) $^ -c -o $@

.PHONY: clean

clean:
	rm -f windows/img.iso
	rm -f kernel
	rm -f src/*.o