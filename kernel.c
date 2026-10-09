#include "kernel.h"
#include "common.h"

extern char __bss[], __bss_end[], __stack_top[];

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3,long arg4, long arg5, long fid, long eid){ // does riscv64 need long long?
  register long a0 __asm__("a0") = arg0;
  register long a1 __asm__("a1") = arg1;
  register long a2 __asm__("a2") = arg2;
  register long a3 __asm__("a3") = arg3;
  register long a4 __asm__("a4") = arg4;

  register long a5 __asm__("a5") = arg5;
  register long a6 __asm__("a6") = fid;
  register long a7 __asm__("a7") = eid;

  __asm__ __volatile__("ecall"
                        : "=r"(a0), "=r"(a1) 
                        : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
                        : "memory"
                        );
  return (struct sbiret){.error = a0, .value = a1};
}

extern char __free_ram[], __end_of_ram[];
paddr_t alloc_page(uint32_t n){
  static paddr_t next_paddr = (paddr_t) __free_ram; //next_paddr is global var
  paddr_t paddr = next_paddr;
  next_paddr += n * PAGE_SIZE;

  if (next_paddr > (paddr_t) __end_of_ram) PANIC("End of memory");

  memset((void *) paddr, 0 , n * PAGE_SIZE);
  return paddr;
}

#include <stdint.h>

__attribute__((naked)) void switch_context(uint64_t *prev_sp,
                                           uint64_t *next_sp) {
    __asm__ __volatile__(
        "addi sp, sp, -112\n" 
        
        "sd ra,  0  * 8(sp)\n"   
        "sd s0,  1  * 8(sp)\n"
        "sd s1,  2  * 8(sp)\n"
        "sd s2,  3  * 8(sp)\n"
        "sd s3,  4  * 8(sp)\n"
        "sd s4,  5  * 8(sp)\n"
        "sd s5,  6  * 8(sp)\n"
        "sd s6,  7  * 8(sp)\n"
        "sd s7,  8  * 8(sp)\n"
        "sd s8,  9  * 8(sp)\n"
        "sd s9,  10 * 8(sp)\n"
        "sd s10, 11 * 8(sp)\n"
        "sd s11, 12 * 8(sp)\n"

        "sd sp, (a0)\n"         // *prev_sp = sp;
        "ld sp, (a1)\n"         // sp = *next_sp; (Chuyển sang stack của tiến trình mới)

        "ld ra,  0  * 8(sp)\n"  
        "ld s0,  1  * 8(sp)\n"
        "ld s1,  2  * 8(sp)\n"
        "ld s2,  3  * 8(sp)\n"
        "ld s3,  4  * 8(sp)\n"
        "ld s4,  5  * 8(sp)\n"
        "ld s5,  6  * 8(sp)\n"
        "ld s6,  7  * 8(sp)\n"
        "ld s7,  8  * 8(sp)\n"
        "ld s8,  9  * 8(sp)\n"
        "ld s9,  10 * 8(sp)\n"
        "ld s10, 11 * 8(sp)\n"
        "ld s11, 12 * 8(sp)\n"
        
        "addi sp, sp, 112\n"  
        "ret\n"
    );
}


void putchar(char ch) {
    sbi_call(ch, 0, 0, 0, 0, 0, 0, 1 /* Console Putchar */);
}

void printf(const char *fmt, ...);

__attribute__((aligned(4))) // stvec 2 first bit is always 00 (or divideable by 4)
                            // https://docs.riscv.org/reference/isa/priv/supervisor.html#12-1-1-2-supervisor-trap-vector-base-address-stvec-register
__attribute__((naked))
void kernel_entry(void) {
    __asm__ __volatile__(
        "csrw sscratch, sp\n"

        "addi sp, sp, -8 * 32\n"

        "sd ra,  8 * 0(sp)\n"
        "sd gp,  8 * 1(sp)\n"
        "sd tp,  8 * 2(sp)\n"
        "sd t0,  8 * 3(sp)\n"
        "sd t1,  8 * 4(sp)\n"
        "sd t2,  8 * 5(sp)\n"
        "sd t3,  8 * 6(sp)\n"
        "sd t4,  8 * 7(sp)\n"
        "sd t5,  8 * 8(sp)\n"
        "sd t6,  8 * 9(sp)\n"
        "sd a0,  8 * 10(sp)\n"
        "sd a1,  8 * 11(sp)\n"
        "sd a2,  8 * 12(sp)\n"
        "sd a3,  8 * 13(sp)\n"
        "sd a4,  8 * 14(sp)\n"
        "sd a5,  8 * 15(sp)\n"
        "sd a6,  8 * 16(sp)\n"
        "sd a7,  8 * 17(sp)\n"
        "sd s0,  8 * 18(sp)\n"
        "sd s1,  8 * 19(sp)\n"
        "sd s2,  8 * 20(sp)\n"
        "sd s3,  8 * 21(sp)\n"
        "sd s4,  8 * 22(sp)\n"
        "sd s5,  8 * 23(sp)\n"
        "sd s6,  8 * 24(sp)\n"
        "sd s7,  8 * 25(sp)\n"
        "sd s8,  8 * 26(sp)\n"
        "sd s9,  8 * 27(sp)\n"
        "sd s10, 8 * 28(sp)\n"
        "sd s11, 8 * 29(sp)\n"

        "csrr a0, sscratch\n"
        "sd a0, 8 * 30(sp)\n"

        "mv a0, sp\n"
        "call handle_trap\n"

        "ld ra,  8 * 0(sp)\n"
        "ld gp,  8 * 1(sp)\n"
        "ld tp,  8 * 2(sp)\n"
        "ld t0,  8 * 3(sp)\n"
        "ld t1,  8 * 4(sp)\n"
        "ld t2,  8 * 5(sp)\n"
        "ld t3,  8 * 6(sp)\n"
        "ld t4,  8 * 7(sp)\n"
        "ld t5,  8 * 8(sp)\n"
        "ld t6,  8 * 9(sp)\n"
        "ld a0,  8 * 10(sp)\n"
        "ld a1,  8 * 11(sp)\n"
        "ld a2,  8 * 12(sp)\n"
        "ld a3,  8 * 13(sp)\n"
        "ld a4,  8 * 14(sp)\n"
        "ld a5,  8 * 15(sp)\n"
        "ld a6,  8 * 16(sp)\n"
        "ld a7,  8 * 17(sp)\n"
        "ld s0,  8 * 18(sp)\n"
        "ld s1,  8 * 19(sp)\n"
        "ld s2,  8 * 20(sp)\n"
        "ld s3,  8 * 21(sp)\n"
        "ld s4,  8 * 22(sp)\n"
        "ld s5,  8 * 23(sp)\n"
        "ld s6,  8 * 24(sp)\n"
        "ld s7,  8 * 25(sp)\n"
        "ld s8,  8 * 26(sp)\n"
        "ld s9,  8 * 27(sp)\n"
        "ld s10, 8 * 28(sp)\n"
        "ld s11, 8 * 29(sp)\n"
        "ld sp,  8 * 30(sp)\n"

        "sret\n"
    );
}
void kernel_main(void) {
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);

    printf("\n\n Hello %s\n", "Muffin");

    paddr_t paddr1 = alloc_page(2);
    paddr_t paddr2 = alloc_page(1);
    printf("Page test paddr1=%x\n", paddr1);
    printf("page test paddr2=%x\n", paddr2);

    WRITE_CSR(stvec, (uint64_t) kernel_entry);
    __asm__ __volatile__("unimp"); // unimp is pseudo code for csrrw x0, cycle, x0
    for (;;){
      __asm__ __volatile__("wfi");
    }
}

void handle_trap(struct trap_frame *f){
  uint64_t scause = READ_CSR(scause);
  uint64_t stval = READ_CSR(stval);
  uint64_t user_pc = READ_CSR(sepc);
  PANIC("unexpected trap scause=%x, stval=%x, sepc=%x\n", scause, stval, user_pc);
}


__attribute__((section(".text.boot")))
__attribute__((naked))

__attribute__((used))
void boot(void) {
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n"
        "j kernel_main\n"
        :
        : [stack_top] "r" (__stack_top)
    );
}
