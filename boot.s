.section .text.init
.global boot

boot:
    # 1. Park secondary cores (hart != 0)
    csrr t0, mhartid
    bnez t0, park

    # 2. Set up stack pointer from linker symbol
    la sp, __stack_top

    # 3. Clear .bss
    la t0, __bss_start
    la t1, __bss_end
clear_bss:
    bge t0, t1, jump_main
    sd zero, (t0)
    addi t0, t0, 8
    j clear_bss

jump_main:
    tail main

park:
    wfi
    j park
