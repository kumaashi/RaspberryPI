ARMGNU ?= arm-none-eabi
AOPS = --warn --fatal-warnings
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding
HEADER=*.h
SOURCE=*.s *.c
all : kernel.img

main.elf : memmap $(SOURCE) $(HEADER)
	$(ARMGNU)-as $(AOPS) vectors.s -o vectors.o
	$(ARMGNU)-gcc $(COPS) -c $(SOURCE)
	$(ARMGNU)-ld *.o -T memmap -o main.elf
	$(ARMGNU)-objdump -D main.elf > main.list

kernel.img : main.elf
	$(ARMGNU)-objcopy --srec-forceS3 main.elf -O srec main.srec
	$(ARMGNU)-objcopy main.elf -O binary kernel.img

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.srec
	rm -f *.elf
	rm -f *.list
	rm -f *.img

