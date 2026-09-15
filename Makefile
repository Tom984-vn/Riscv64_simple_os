CC = riscv64-elf-gcc
CFLAGS = -nostdlib -fno-builtin -mcmodel=medany -Wall -O2 -march=rv64gcv -mabi=lp64d

SRCS = kernel.c common.c
TARGET = kernel.elf

all: $(TARGET)

$(TARGET): $(SRCS) linker.ld
	$(CC) $(CFLAGS) -T linker.ld $(SRCS) -o $(TARGET)

run: $(TARGET)
	qemu-system-riscv64 -machine virt -nographic -bios default -cpu rv64,v=true,vlen=128,elen=64 -kernel $(TARGET)

clean:
	rm -f $(TARGET)
