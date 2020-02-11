#CFLAGS=-mcpu=cortex-a72 -mfpu=neon-vfpv4 -O2 -ffreestanding -std=gnu99 -nostdlib -mfloat-abi=softfp
#CFLAGS=-mcpu=cortex-a72  -O2 -ffreestanding -std=gnu99 -nostdlib 
CFLAGS=-fno-builtin -std=c11 -Wall -Wextra -nostartfiles -nostdlib -Wl,--gc-sections

#ARCH=arm-none-eabi
ARCH=aarch64-linux-gnu
GCC=$(ARCH)-gcc

OBJCOPY=$(ARCH)-objcopy
OBJDUMP=$(ARCH)-objdump
MEMORYMAP=ldscript
HEADER=*.h
SOURCE=boot.s main.c gpio.c uart.c

image.bin: $(SOURCE) $(HEADER)
	$(GCC) $(CFLAGS) -c $(SOURCE)
	$(GCC) $(CFLAGS) -T $(MEMORYMAP).ld -o image.elf *.o 
	$(OBJCOPY) -O binary image.elf image.bin
	$(OBJDUMP) -D image.elf 2>&1 | tee asmlist.txt

clean:
	rm *.o *.elf *.txt *.bin
