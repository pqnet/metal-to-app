
CC=clang-9
ISOFILE=windows/img.iso

TARGET=x86_64-none-elf
FLAGS=-nostdlib -static -target $(TARGET)
LINK_FLAGS=-fno-exceptions -fno-unwind-tables -ffreestanding -z max-page-size=4096 -Wl,-n -Wl,--build-id=none

$(ISOFILE): kernel iso/boot/grub/grub.cfg
	cp kernel iso
	grub-mkrescue -d /usr/lib/grub/i386-pc -o $@ iso

kernel: linker.ld helloworld.o main.o
	$(CC) $(FLAGS) $(LINK_FLAGS) -Wl,-T,$^ -o $@
	grub-file --is-x86-multiboot $@

helloworld.o: helloworld.s
	$(CC) $(FLAGS) $^ -c -o $@
 
main.o: main.c
	$(CC) $(FLAGS) $^ -c -o $@

.PHONY: clean

clean:
	rm -f windows/img.iso
	rm -f kernel
	rm -f helloworld.o
	rm -f main.o