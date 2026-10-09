#pragma once
#include "common.h"

struct trap_frame {
    uint64_t ra;
    uint64_t gp;
    uint64_t tp;
    uint64_t t0;
    uint64_t t1;
    uint64_t t2;
    uint64_t t3;
    uint64_t t4;
    uint64_t t5;
    uint64_t t6;
    uint64_t a0;
    uint64_t a1;
    uint64_t a2;
    uint64_t a3;
    uint64_t a4;
    uint64_t a5;
    uint64_t a6;
    uint64_t a7;
    uint64_t s0;
    uint64_t s1;
    uint64_t s2;
    uint64_t s3;
    uint64_t s4;
    uint64_t s5;
    uint64_t s6;
    uint64_t s7;
    uint64_t s8;
    uint64_t s9;
    uint64_t s10;
    uint64_t s11;
    uint64_t sp;
} __attribute__((packed));

// (string A : string B : string C)
// string A is the asm code
// string B is the output
// string C is the input

// below are multiline macro, usually use for debugging for something that doesn't know the data type
// require "\" at the end of every line, except the last line
#define READ_CSR(reg) ({ \
      unsigned long long __tmp; \
      __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
      __tmp; \
    })

#define WRITE_CSR(reg, value) \
do { \
  uint64_t __tmp = (value); \
  __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp)); \
} while (0) 

#define PANIC(fmt, ...) \
do { \
  printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
  while (1) {} \
} while (0)

struct sbiret {
    long error;
    long value;
};

#define PROCS_MAX 8

#define PROC_USED 1
#define PROC_UNUSED 0

struct process {
    int pid;             // Process ID
    int state;           // Process state: PROC_UNUSED or PROC_RUNNABLE
    vaddr_t sp;          // Stack pointer
    uint8_t stack[8192]; // Kernel stack
};

