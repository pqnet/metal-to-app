
CC=clang-9
ISOFILE=windows/img.iso

TARGET=x86_64-none-elf
FLAGS=-Werror -Wall -Os -nostdlib -static -target $(TARGET)
ARCH_DIR=src/arch/x86_64/
CFLAGS=-fno-asynchronous-unwind-tables -std=gnu2x -mcmodel=kernel -mno-red-zone -mno-sse -I./src -I./$(ARCH_DIR)

LINK_FLAGS=-fno-exceptions -fno-unwind-tables -ffreestanding -Wl,-n -Wl,--build-id=none

$(ISOFILE): kernel testelf iso/boot/grub/grub.cfg
	cp kernel iso
	cp testelf iso
	grub-mkrescue -d /usr/lib/grub/i386-pc -o $@ iso

ARCH_SRCS=$(addprefix $(ARCH_DIR),\
multiboot.c\
interrupts.c\
exceptions.c\
gdt.c\
)
ARCH_OBJECTS=$(ARCH_SRCS:%.c=%.o)
SRCS=\
src/memset.c src/memcpy.c \
src/initrd.c src/elf.c src/userspace.c\
src/bootstrap.c src/print.c\
src/keyboard.c src/scancodes.c\
src/scheduler.c src/scheduler_asm.s src/test_scheduler.c\
src/frame.c src/memory.c src/address_space.c\
src/syscall_asm.c src/syscall.c src/syscall_table.c\
src/main.c
OBJECTS=$(patsubst %.s, %.o, $(SRCS:.c=.o))

kernel: linker.ld $(OBJECTS) $(ARCH_OBJECTS) testelf
	$(CC) $(FLAGS) $(LINK_FLAGS) -Wl,-T,linker.ld $(OBJECTS) $(ARCH_OBJECTS) -o $@
	# objcopy --add-gnu-debuglink=testelf kernel
	grub-file --is-x86-multiboot $@

.PHONY: clean

DEPDIR:=.deps
DEPFILES:=$(patsubst %,$(DEPDIR)/%,$(notdir $(filter-out %.s,$(SRCS:%.c=/%.d))))

clean:
	rm -f windows/img.iso
	rm -f kernel
	rm -f testelf
	rm -f src/*.o
	rm -f modules/*/src/*.o
	rm -f $(OBJECTS)
	rm -f $(ARCH_OBJECTS)
	rm -f $(DEPFILES)

	
testelf: modules/testelf/src/testelf.o modules/vdso/src/vdso.o
	$(CC) $(FLAGS) $(LINK_FLAGS) $^ -o $@


DEPFLAGS = -MT $@ -MMD -MF $(DEPDIR)/$(notdir $*.d)

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(FLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.s = $(CC) $(FLAGS) $(TARGET_ARCH) -c

%.o: %.c
%.o: %.c | $(DEPDIR)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

$(DEPDIR):
	@mkdir $@

-include $(DEPFILES)