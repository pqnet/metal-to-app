OUTPUT_FORMAT(elf64-x86-64)
/*OUTPUT_FORMAT(elf32-x86-64) */
SECTIONS {
    /* kernel starts at -2gb  = - 80000000*/
    KERNEL_BASE = 0xffffffff80000000;
    ENTRY(_start)
    /* first, we put bootloader at 1mb virtual address */
    . = 0x100000;
    .text0 : {
        *bootstrap.o(.multiboot)
        *bootstrap.o(.text)
    }
    .data0 : {
        *bootstrap.o(.data)
    }
    . = ALIGN(0x200000);
    .BOOTLOADER_END = .;

    /* we put kernel in the next available 2mb page after bootloader */
    . +=KERNEL_BASE;
    .text : AT(ADDR(.text) - KERNEL_BASE)  {
        *(.text*)
    }
    .rodata : AT(ADDR(.rodata) - KERNEL_BASE) {
        *(.rodata*)
    }
    . = KERNEL_BASE + 8 * 0x200000;
    . += 0x200000;
    STACK_BOTTOM = .;
    .data : AT(ADDR(.data) - KERNEL_BASE) {
        *(.data*)
    }
    .bss : AT(ADDR(.bss) - KERNEL_BASE) {
        *(.bss*)
    }
    FREEMEMORY_START= ALIGN(0x200000) - KERNEL_BASE;
}