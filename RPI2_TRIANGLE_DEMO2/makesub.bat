del *.o a.out *.elf *.img

REM SET CFLAGS=-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -Os -mtune=cortex-a7 -std=gnu99 -nostdlib -ffreestanding
SET CFLAGS=-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -O3 -mtune=cortex-a7 -std=gnu99 -nostdlib -ffreestanding
SET CPPFLAGS=-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -O3 -mtune=cortex-a7 -nostdlib -ffreestanding

SET GCC=arm-none-eabi-gcc
SET GPP=arm-none-eabi-g++
SET LD=arm-none-eabi-ld
SET OBJCOPY=arm-none-eabi-objcopy
SET OBJDUMP=arm-none-eabi-objdump
SET READELF=arm-none-eabi-readelf

%GCC%     %CFLAGS%   -c *.s
%GCC%     %CFLAGS%   -c *.c  -Wall
%LD%     -T memorymap.ld -o image.elf -nostdlib *.o
%READELF% -a image.elf > ELFMEMORY_MAP.TXT
%OBJCOPY% -O binary image.elf  image.bin
%OBJDUMP% image.elf -D > asmlist.txt
arm-none-eabi-readelf -a image.elf > image.txt



