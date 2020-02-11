CFLAGS=-mcpu=cortex-a7 -mfpu=neon-vfpv4 -O2 -ffreestanding -std=gnu99 -nostdlib -mfloat-abi=softfp
GCC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
HEADER=*.h
SOURCE=*.s *.c
INSTALL="/Volumes/NO NAME"

image.bin: $(SOURCE) $(HEADER)
	$(GCC) $(CFLAGS) -c $(SOURCE)
	$(GCC) $(CFLAGS) -T memorymap.ld -o image.elf -nostdlib *.o -lm
	$(OBJCOPY) -O binary image.elf image.bin
	$(OBJDUMP) -D image.elf 2>&1 | tee asmlist.txt 

clean:
	rm *.o *.elf *.out *.txt *.bin

install:
	cp image.bin $(INSTALL)/image.bin 
