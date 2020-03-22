CFLAGS=-fno-builtin -std=c11 -nostartfiles -nostdlib -Wl,--gc-sections -O2
ARCH=aarch64-linux-gnu
GCC=$(ARCH)-gcc
OBJCOPY=$(ARCH)-objcopy
OBJDUMP=$(ARCH)-objdump
MEMORYMAP=ldscript
HEADER=*.h
SOURCE=boot.s main.c gpio.c uart.c pcie.c mailbox.c std.c

image.bin: $(SOURCE) $(HEADER)
	$(GCC) $(CFLAGS) -c $(SOURCE)
	$(GCC) $(CFLAGS) -T $(MEMORYMAP).ld -o image.elf *.o 
	$(OBJCOPY) -O binary image.elf image.bin
	$(OBJDUMP) -D image.elf 2>&1 | tee asmlist.txt
clean:
	rm *.o *.elf *.txt *.bin
