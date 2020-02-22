OUTPUT_FORMAT(elf64-x86-64)
/*OUTPUT_FORMAT(elf32-x86-64) */
SECTIONS {
    /* kernel starts at -2gb  = - 80000000*/
    KERNEL_BASE = 0xffffffff80000000;
    ENTRY(_start)
    . = 0x100000;
    .text0 : {
        *bootstrap.o(.multiboot)
        *bootstrap.o(.text)
    }
    . = ALIGN(0x200000);
    .BOOTLOADER_END = .;
    . = 8 * 0x200000;
    .RO_END = .;
    . += 0x400000;
    .data0 : { *bootstrap.o(.data) }
    .DATA_END = .;
    . = KERNEL_BASE + .BOOTLOADER_END;
    .text : AT(ADDR(.text) - KERNEL_BASE)  {
        *(.text*)
    }
    . = ALIGN(0x200000);
    .rodata : AT(ADDR(.rodata) - KERNEL_BASE) {
        *(.rodata*)
    }
    . = KERNEL_BASE + .RO_END;
    .bss0 : {
        .stack_top = .;
        . += 0x400000;
        .stack_bottom = .;
    }
    . = KERNEL_BASE + .DATA_END;
    . = ALIGN(0x200000);
    .data : AT(ADDR(.data) - KERNEL_BASE) {
        *(.data*)
    }
    .bss : AT(ADDR(.bss) - KERNEL_BASE) {
        *(.bss*)
    }
    FREEMEMORY_START= ALIGN(0x200000) - KERNEL_BASE;
}