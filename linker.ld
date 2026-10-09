ENTRY(boot)

SECTIONS {
    . = 0x80200000;

    .text :{
        KEEP(*(.text.boot));
        *(.text .text.*);
    }

    .rodata : ALIGN(8) {
        *(.rodata .rodata.*);
    }

    .data : ALIGN(8) {
        *(.data .data.*);
    }

    .bss : ALIGN(8) {
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        __bss_end = .;
    }

    . = ALIGN(16); /* sp stack req 16 bytes align for some reason */
    . += 128 * 1024; /* 128KB */
    __stack_top = .;

    . = ALIGN(4096); /* align 4KB for free ram */
    __free_ram = .;
    . += 64 * 1024 * 1024;
    __end_of_ram = .;
}
