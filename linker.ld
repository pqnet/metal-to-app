OUTPUT_FORMAT(elf64-x86-64)
/*OUTPUT_FORMAT(elf32-x86-64) */
SECTIONS {
    ENTRY(_start)
    . = 0x100000;
    .text0 : {
        *(.multiboot)
        *(.text)
    }
    . = 4 * 0x200000;
    .data0 : { *(.data) }
    .bss0 : { *(.bss) }
}